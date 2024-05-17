static bool flush_incoming_que_on_wr_signal(rfc_slot_t *slot) {
 while (!list_is_empty(slot->incoming_queue)) {
    BT_HDR *p_buf = list_front(slot->incoming_queue);
 switch (send_data_to_app(slot->fd, p_buf)) {
 case SENT_NONE:
 case SENT_PARTIAL:
        btsock_thread_add_fd(pth, slot->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_WR, slot->id);
 return true;

 case SENT_ALL:
        list_remove(slot->incoming_queue, p_buf);
 break;

 case SENT_FAILED:
        list_remove(slot->incoming_queue, p_buf);
 return false;
 }
 }

  APPL_TRACE_DEBUG("enable data flow, rfc_handle:0x%x, rfc_port_handle:0x%x, user_id:%d",
      slot->rfc_handle, slot->rfc_port_handle, slot->id);
 extern int PORT_FlowControl_MaxCredit(uint16_t handle, bool enable);
  PORT_FlowControl_MaxCredit(slot->rfc_port_handle, true);
 return true;
}
