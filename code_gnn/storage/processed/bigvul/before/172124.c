static void do_postload() {
  LOG_DEBUG("%s posting postload work item", __func__);
  thread_post(thread, event_postload, NULL);
}
