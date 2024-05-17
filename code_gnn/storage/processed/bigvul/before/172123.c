static void dispatch_reassembled(BT_HDR *packet) {
  assert((packet->event & MSG_EVT_MASK) != MSG_HC_TO_STACK_HCI_EVT);
  assert(upwards_data_queue != NULL);

 if (upwards_data_queue) {
    fixed_queue_enqueue(upwards_data_queue, packet);
 } else {
    LOG_ERROR("%s had no queue to place upwards data packet in. Dropping it on the floor.", __func__);
    buffer_allocator->free(packet);
 }
}
