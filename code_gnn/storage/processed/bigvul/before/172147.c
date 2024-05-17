static void transmit_fragment(BT_HDR *packet, bool send_transmit_finished) {
 uint16_t event = packet->event & MSG_EVT_MASK;
 serial_data_type_t type = event_to_data_type(event);

  btsnoop->capture(packet, false);
  hal->transmit_data(type, packet->data + packet->offset, packet->len);

 if (event != MSG_STACK_TO_HC_HCI_CMD && send_transmit_finished)
    buffer_allocator->free(packet);
}
