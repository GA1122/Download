static void on_rfc_close(UNUSED_ATTR tBTA_JV_RFCOMM_CLOSE *p_close, uint32_t id) {
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (slot)
    cleanup_rfc_slot(slot);

  pthread_mutex_unlock(&slot_lock);
}
