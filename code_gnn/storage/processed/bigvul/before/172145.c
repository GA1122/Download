static future_t *transmit_command_futured(BT_HDR *command) {
 waiting_command_t *wait_entry = osi_calloc(sizeof(waiting_command_t));
  assert(wait_entry != NULL);

 future_t *future = future_new();

 uint8_t *stream = command->data + command->offset;
  STREAM_TO_UINT16(wait_entry->opcode, stream);
  wait_entry->complete_future = future;
  wait_entry->command = command;

  command->event = MSG_STACK_TO_HC_HCI_CMD;

  fixed_queue_enqueue(command_queue, wait_entry);
 return future;
}
