int bta_co_rfc_data_incoming(void *user_data, BT_HDR *p_buf) {
  pthread_mutex_lock(&slot_lock);

 int ret = 0;
 uint32_t id = (uintptr_t)user_data;
 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (!slot)
 goto out;

 if (list_is_empty(slot->incoming_queue)) {
 switch (send_data_to_app(slot->fd, p_buf)) {
 case SENT_NONE:
 case SENT_PARTIAL:
        list_append(slot->incoming_queue, p_buf);
        btsock_thread_add_fd(pth, slot->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_WR, slot->id);
 break;

 case SENT_ALL:
        GKI_freebuf(p_buf);
        ret = 1;  
 break;

 case SENT_FAILED:
        GKI_freebuf(p_buf);
        cleanup_rfc_slot(slot);
 break;
 }
 } else {
    list_append(slot->incoming_queue, p_buf);
 }

out:;
  pthread_mutex_unlock(&slot_lock);
 return ret;  
}
