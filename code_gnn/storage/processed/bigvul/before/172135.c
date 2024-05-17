static void fragmenter_transmit_finished(BT_HDR *packet, bool all_fragments_sent) {
 if (all_fragments_sent) {
    buffer_allocator->free(packet);
 } else {
    data_dispatcher_dispatch(interface.event_dispatcher, packet->event & MSG_EVT_MASK, packet);
 }
}
