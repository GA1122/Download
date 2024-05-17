static void *watchdog_fn(void *arg) {
 int current_id = 0;
 for (;;) {
 for (int i = 0; watchdog_running && i < WATCHDOG_PERIOD_SEC; ++i) {
      sleep(1);
 }

 if (!watchdog_running)
 break;

 if (current_id == watchdog_id) {
      printf("Watchdog detected hanging test suite, aborting...\n");
      exit(-1);
 }
    current_id = watchdog_id;
 }
 return NULL;
}
