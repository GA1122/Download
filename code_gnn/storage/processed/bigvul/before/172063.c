static void on_rfc_outgoing_congest(tBTA_JV_RFCOMM_CONG *p, uint32_t id) {
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (slot) {
    slot->f.outgoing_congest = p->cong ? 1 : 0;
 if (!slot->f.outgoing_congest)
      btsock_thread_add_fd(pth, slot->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_RD, slot->id);
 }

  pthread_mutex_unlock(&slot_lock);
}
