static void cb(UNUSED_ATTR void *data) {
 ++cb_counter;
  semaphore_post(semaphore);
}
