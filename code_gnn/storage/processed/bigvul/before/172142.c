static future_t *shut_down() {
  LOG_INFO("%s", __func__);

  hci_inject->close();

 if (thread) {
 if (firmware_is_configured) {
      non_repeating_timer_restart(epilog_timer);
      thread_post(thread, event_epilog, NULL);
 } else {
      thread_stop(thread);
 }

    thread_join(thread);
 }

  fixed_queue_free(command_queue, osi_free);
  fixed_queue_free(packet_queue, buffer_allocator->free);
  list_free(commands_pending_response);

  pthread_mutex_destroy(&commands_pending_response_lock);

  packet_fragmenter->cleanup();

  non_repeating_timer_free(epilog_timer);
  non_repeating_timer_free(command_response_timer);
  non_repeating_timer_free(startup_timer);

  epilog_timer = NULL;
  command_response_timer = NULL;

  low_power_manager->cleanup();
  hal->close();

 int power_state = BT_VND_PWR_OFF;
  vendor->send_command(VENDOR_CHIP_POWER_CONTROL, &power_state);
  vendor->close();

  thread_free(thread);
  thread = NULL;
  firmware_is_configured = false;

 return NULL;
}
