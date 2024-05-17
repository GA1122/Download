static void firmware_config_callback(UNUSED_ATTR bool success) {
  LOG_INFO("%s", __func__);
  firmware_is_configured = true;
  non_repeating_timer_cancel(startup_timer);

  future_ready(startup_future, FUTURE_SUCCESS);
  startup_future = NULL;
}
