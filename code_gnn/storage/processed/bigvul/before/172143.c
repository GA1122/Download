static future_t *start_up(void) {
  LOG_INFO("%s", __func__);

  command_credits = 1;
  firmware_is_configured = false;

  pthread_mutex_init(&commands_pending_response_lock, NULL);

 period_ms_t startup_timeout_ms;
 char timeout_prop[PROPERTY_VALUE_MAX];
 if (!property_get("bluetooth.enable_timeout_ms", timeout_prop, STRING_VALUE_OF(DEFAULT_STARTUP_TIMEOUT_MS))
 || (startup_timeout_ms = atoi(timeout_prop)) < 100)
    startup_timeout_ms = DEFAULT_STARTUP_TIMEOUT_MS;

  startup_timer = non_repeating_timer_new(startup_timeout_ms, startup_timer_expired, NULL);
 if (!startup_timer) {
    LOG_ERROR("%s unable to create startup timer.", __func__);
 goto error;
 }

  non_repeating_timer_restart(startup_timer);

  epilog_timer = non_repeating_timer_new(EPILOG_TIMEOUT_MS, epilog_timer_expired, NULL);
 if (!epilog_timer) {
    LOG_ERROR("%s unable to create epilog timer.", __func__);
 goto error;
 }

  command_response_timer = non_repeating_timer_new(COMMAND_PENDING_TIMEOUT, command_timed_out, NULL);
 if (!command_response_timer) {
    LOG_ERROR("%s unable to create command response timer.", __func__);
 goto error;
 }

  command_queue = fixed_queue_new(SIZE_MAX);
 if (!command_queue) {
    LOG_ERROR("%s unable to create pending command queue.", __func__);
 goto error;
 }

  packet_queue = fixed_queue_new(SIZE_MAX);
 if (!packet_queue) {
    LOG_ERROR("%s unable to create pending packet queue.", __func__);
 goto error;
 }

  thread = thread_new("hci_thread");
 if (!thread) {
    LOG_ERROR("%s unable to create thread.", __func__);
 goto error;
 }

  commands_pending_response = list_new(NULL);
 if (!commands_pending_response) {
    LOG_ERROR("%s unable to create list for commands pending response.", __func__);
 goto error;
 }

  memset(incoming_packets, 0, sizeof(incoming_packets));

  packet_fragmenter->init(&packet_fragmenter_callbacks);

  fixed_queue_register_dequeue(command_queue, thread_get_reactor(thread), event_command_ready, NULL);
  fixed_queue_register_dequeue(packet_queue, thread_get_reactor(thread), event_packet_ready, NULL);

  vendor->open(btif_local_bd_addr.address, &interface);
  hal->init(&hal_callbacks, thread);
  low_power_manager->init(thread);

  vendor->set_callback(VENDOR_CONFIGURE_FIRMWARE, firmware_config_callback);
  vendor->set_callback(VENDOR_CONFIGURE_SCO, sco_config_callback);
  vendor->set_callback(VENDOR_DO_EPILOG, epilog_finished_callback);

 if (!hci_inject->open(&interface)) {
 }

 int power_state = BT_VND_PWR_OFF;
#if (defined (BT_CLEAN_TURN_ON_DISABLED) && BT_CLEAN_TURN_ON_DISABLED == TRUE)
  LOG_WARN("%s not turning off the chip before turning on.", __func__);

#else
  vendor->send_command(VENDOR_CHIP_POWER_CONTROL, &power_state);
#endif
  power_state = BT_VND_PWR_ON;
  vendor->send_command(VENDOR_CHIP_POWER_CONTROL, &power_state);

  startup_future = future_new();
  LOG_DEBUG("%s starting async portion", __func__);
  thread_post(thread, event_finish_startup, NULL);
 return startup_future;
error:;
  shut_down();  
 return future_new_immediate(FUTURE_FAIL);
}
