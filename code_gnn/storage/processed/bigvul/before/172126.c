static void epilog_timer_expired(UNUSED_ATTR void *context) {
  LOG_INFO("%s", __func__);
  thread_stop(thread);
}
