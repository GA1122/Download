static bool filter_incoming_event(BT_HDR *packet) {
 waiting_command_t *wait_entry = NULL;
 uint8_t *stream = packet->data;
 uint8_t event_code;
 command_opcode_t opcode;

  STREAM_TO_UINT8(event_code, stream);
  STREAM_SKIP_UINT8(stream);  

 if (event_code == HCI_COMMAND_COMPLETE_EVT) {
    STREAM_TO_UINT8(command_credits, stream);
    STREAM_TO_UINT16(opcode, stream);

    wait_entry = get_waiting_command(opcode);
 if (!wait_entry)
      LOG_WARN("%s command complete event with no matching command. opcode: 0x%x.", __func__, opcode);
 else if (wait_entry->complete_callback)
      wait_entry->complete_callback(packet, wait_entry->context);
 else if (wait_entry->complete_future)
      future_ready(wait_entry->complete_future, packet);

 goto intercepted;
 } else if (event_code == HCI_COMMAND_STATUS_EVT) {
 uint8_t status;
    STREAM_TO_UINT8(status, stream);
    STREAM_TO_UINT8(command_credits, stream);
    STREAM_TO_UINT16(opcode, stream);


    wait_entry = get_waiting_command(opcode);
 if (!wait_entry)
      LOG_WARN("%s command status event with no matching command. opcode: 0x%x", __func__, opcode);
 else if (wait_entry->status_callback)
      wait_entry->status_callback(status, wait_entry->command, wait_entry->context);

 goto intercepted;
 }

 return false;
intercepted:;
  non_repeating_timer_restart_if(command_response_timer, !list_is_empty(commands_pending_response));

 if (wait_entry) {
 if (event_code == HCI_COMMAND_STATUS_EVT || (!wait_entry->complete_callback && !wait_entry->complete_future))
      buffer_allocator->free(packet);

 if (event_code == HCI_COMMAND_COMPLETE_EVT || !wait_entry->status_callback)
      buffer_allocator->free(wait_entry->command);

    osi_free(wait_entry);
 } else {
    buffer_allocator->free(packet);
 }

 return true;
}
