static void transmit_downward(data_dispatcher_type_t type, void *data) {
 if (type == MSG_STACK_TO_HC_HCI_CMD) {
    transmit_command((BT_HDR *)data, NULL, NULL, NULL);
    LOG_WARN("%s legacy transmit of command. Use transmit_command instead.", __func__);
 } else {
    fixed_queue_enqueue(packet_queue, data);
 }
}
