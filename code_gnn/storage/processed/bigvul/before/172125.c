static void epilog_finished_callback(UNUSED_ATTR bool success) {
  LOG_INFO("%s", __func__);
  thread_stop(thread);
}
