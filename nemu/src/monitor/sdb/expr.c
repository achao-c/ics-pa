#include <isa.h>

/* We use the POSIX regex functions to process regular expressions.
 * Type 'man regex' for more information about POSIX regex functions.
 */
#include <regex.h>

enum {
  TK_NOTYPE = 256, TK_EQ, NUM, HEX, TK_NEQ, TK_AND

  /* TODO: Add more token types */

};

static struct rule {
  const char *regex;
  int token_type;
} rules[] = {

  /* TODO: Add more rules.
   * Pay attention to the precedence level of different rules.
   */

  {" +", TK_NOTYPE},    // spaces
  {"\\+", '+'},         // plus
  {"-", '-'},           // substract
  {"\\*", '*'},         // mul
  {"/", '/'},           // div
  {"0[xX][0-9a-f]+", HEX}, // hex
  {"[0-9]+", NUM},      // number
  {"\\(", '('},         // (
  {"\\)", ')'},         // )
  {"==", TK_EQ},        // equal
  {"!=", TK_NEQ},        // not equal
  {"&&", TK_AND},        // and
  {"\\$[\\$a-z][0-9]+", TK_AND},        // and
};

#define NR_REGEX ARRLEN(rules)

static regex_t re[NR_REGEX] = {};

/* Rules are used for many times.
 * Therefore we compile them only once before any usage.
 */
void init_regex() {
  int i;
  char error_msg[128];
  int ret;

  for (i = 0; i < NR_REGEX; i ++) {
    ret = regcomp(&re[i], rules[i].regex, REG_EXTENDED);
    if (ret != 0) {
      regerror(ret, &re[i], error_msg, 128);
      panic("regex compilation failed: %s\n%s", error_msg, rules[i].regex);
    }
  }
}

typedef struct token {
  int type;
  char str[32];
} Token;

static Token tokens[32] __attribute__((used)) = {};
static int nr_token __attribute__((used))  = 0;

// 
static bool make_token(char *e) {
  int position = 0;
  int i;
  regmatch_t pmatch;

  nr_token = 0;

  while (e[position] != '\0') {
    /* Try all rules one by one. */
    for (i = 0; i < NR_REGEX; i ++) {
      if (regexec(&re[i], e + position, 1, &pmatch, 0) == 0 && pmatch.rm_so == 0) {
        char *substr_start = e + position;
        int substr_len = pmatch.rm_eo;

        Log("match rules[%d] = \"%s\" at position %d with len %d: %.*s",
            i, rules[i].regex, position, substr_len, substr_len, substr_start);

        position += substr_len;

        /* TODO: Now a new token is recognized with rules[i]. Add codes
         * to record the token in the array `tokens'. For certain types
         * of tokens, some extra actions should be performed.
         */

        switch (rules[i].token_type) {
          case TK_NOTYPE:
            break;
          case '+':
            tokens[nr_token++].type = '+';
            break;
          case '-':
            tokens[nr_token++].type = '-';
            break;
          case '*':
            tokens[nr_token++].type = '*';
            break;
          case '/':
            tokens[nr_token++].type = '/';
            break;
          case TK_EQ:
            tokens[nr_token++].type = TK_EQ;
            break;
          case TK_NEQ:
            tokens[nr_token++].type = TK_NEQ;
            break;  
          case TK_AND:
            tokens[nr_token++].type = TK_AND;
            break;
          case '(':
            tokens[nr_token++].type = '(';
            break;  
          case ')':
            tokens[nr_token++].type = ')';
            break;
          case NUM:   // 将字符放入解析数组中
            assert(substr_len <= 31);   // 否则就要发生缓冲区溢出
            tokens[nr_token].type = NUM;
            strncpy(tokens[nr_token].str, substr_start, substr_len); // 拷贝字符串进入数组中
            tokens[nr_token].str[substr_len] = '\0';
            ++nr_token;
            break;
          case HEX:   // 将字符放入解析数组中
            assert(substr_len <= 31);   // 否则就要发生缓冲区溢出
            tokens[nr_token].type = NUM;
            char num[32]; // 将16进制字符取出放入其中，方便转换为10进制
            strncpy(num, substr_start+2, substr_len-2); // 拷贝字符串进入数组中
            num[substr_len-2] = '\0';
            unsigned int true_num = 0;
            sscanf(num, "%x", &true_num);   // 这个函数很有用，可以将不同进制的字符串转换成数字
            sprintf(num, "%d", true_num);
            strncpy(tokens[nr_token].str, num, 32);
            tokens[nr_token].str[strlen(num)] = '\0';
            ++nr_token;
            break;
          default: TODO();
        }

        break;
      }
    }

    if (i == NR_REGEX) {
      printf("no match at position %d\n%s\n%*.s^\n", position, e, position, "");
      return false;
    }
  }

  return true;
}

bool check_parentheses(size_t p, size_t q) {
  // 检查最外层是否为括号
  bool flag = true;
  if (tokens[p].type != '(' || tokens[q].type != ')') flag = false;
  int sum = 0;
  for (size_t idx = p; idx <= q; ++idx) {
    if (tokens[idx].type != '(' && tokens[idx].type != ')') continue;
    if (tokens[idx].type == '(') ++sum;
    else --sum;
    //Log("%d\n ", sum);
    // 1.若sum小于0，一定不合法
    assert(sum >= 0);
    // 2.只有sum为0时位置刚好为q最外层才为括号
    if (sum == 0 && (idx != q)) flag = false; 
  }
  return flag;
}

// priority num 
static struct op_prio {
  const int op;
  int prio;
} op_priority[] = {
  {'+', 1},    
  {'-', 1},
  {'*', 2},
  {'/', 2},
  {TK_EQ, 0},
  {TK_NEQ, 0},
  {TK_AND, -1},
};


size_t find_main_op(size_t p, size_t q) {
  bool findmain = false;
  size_t re_idx = 0;
  // 找出表达式的主运算符
  for (size_t idx = p; idx <= q; ++idx) {
    if (tokens[idx].type == NUM) continue;
    if (tokens[idx].type == '(') {
      while (idx <= q && tokens[idx].type != ')') ++idx;
    }
    // 上面逻辑使得遇到)跳过，所以不会出现)
    else {
      if (!findmain) {
        findmain = true;
        re_idx = idx;
      }
      else {
        int mainprio = 0, nowprio = 0;
        for (int i = 0; i < 6; ++i) {
          if (tokens[re_idx].type == op_priority[i].op) mainprio = op_priority[i].prio;
          if (tokens[idx].type == op_priority[i].op) nowprio = op_priority[i].prio;
        }
        if (nowprio <= mainprio) re_idx = idx;
      }
    }
  }
  return re_idx;
}

u_int32_t eval(size_t p, size_t q) {
  //Log("%zu  %zu", p, q);
  if (p > q) assert(0);
  // 2.现阶段情况只有可能是数字
  else if (p == q) {
    u_int32_t only_num;
    sscanf(tokens[p].str, "%d", &only_num);
    return only_num;
  }  
  // 3.最外面有层括号，将括号拿去
  else if (check_parentheses(p, q)) {
    return eval(p+1, q-1);
  }
  // 4.找到主运算符
  else {
    size_t idx = find_main_op(p, q); 
    u_int32_t left_val = eval(p, idx-1);
    u_int32_t right_val = eval(idx+1, q);

    switch (tokens[idx].type) {
      case '+': return left_val + right_val;
      case '-': return left_val - right_val;
      case '*': return left_val * right_val;
      case '/': return left_val / right_val;
      case TK_EQ: {if (left_val == 0 && right_val == 0) return 1; else if (left_val == 0 || right_val == 0) return 0; return 1;}
      case TK_NEQ: {if (left_val == 0 && right_val == 0) return 0; else if (left_val == 0 || right_val == 0) return 1; return 0;}
      case TK_AND: {if (left_val == 0 || right_val == 0) return 0; return 1;}

    }
  }
  return 0;
}

word_t expr(char *e, bool *success) {
  if (!make_token(e)) {
    *success = false;
    return 0;
  }

  /* TODO: Insert codes to evaluate the expression. */
  return eval(0, nr_token-1);


}
