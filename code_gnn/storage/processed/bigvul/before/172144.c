static void startup_timer_expired(UNUSED_ATTR void *context) {
  LOG_ERROR("%s", __func__);
  future_ready(startup_future, FUTURE_FAIL);
  startup_future = NULL;
}
