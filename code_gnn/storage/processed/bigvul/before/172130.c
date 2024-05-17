static void event_packet_ready(fixed_queue_t *queue, UNUSED_ATTR void *context) {
  BT_HDR *packet = (BT_HDR *)fixed_queue_dequeue(queue);

  low_power_manager->wake_assert();
  packet_fragmenter->fragment_and_dispatch(packet);
  low_power_manager->transmit_done();
}
