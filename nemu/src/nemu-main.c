#include <common.h>

void init_monitor(int, char *[]);
void am_init_monitor();
void engine_start();
int is_exit_status_bad();

int main(int argc, char *argv[]) {
  /* Initialize the monitor. */
#ifdef CONFIG_TARGET_AM
  am_init_monitor();
#else
  // 装载简易程序
  init_monitor(argc, argv);
#endif

  /* Start engine. */
  // 启动调试器
  engine_start();

  return !is_exit_status_bad();
}
