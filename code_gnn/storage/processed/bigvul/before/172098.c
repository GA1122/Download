static void capture(const BT_HDR *buffer, bool is_received) {
 const uint8_t *p = buffer->data + buffer->offset;

  btsnoop_mem_capture(buffer);

 if (logfile_fd == INVALID_FD)
 return;

 switch (buffer->event & MSG_EVT_MASK) {
 case MSG_HC_TO_STACK_HCI_EVT:
      btsnoop_write_packet(kEventPacket, p, false);
 break;
 case MSG_HC_TO_STACK_HCI_ACL:
 case MSG_STACK_TO_HC_HCI_ACL:
      btsnoop_write_packet(kAclPacket, p, is_received);
 break;
 case MSG_HC_TO_STACK_HCI_SCO:
 case MSG_STACK_TO_HC_HCI_SCO:
      btsnoop_write_packet(kScoPacket, p, is_received);
 break;
 case MSG_STACK_TO_HC_HCI_CMD:
      btsnoop_write_packet(kCommandPacket, p, true);
 break;
 }
}
