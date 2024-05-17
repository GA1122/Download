static void on_srv_rfc_listen_started(tBTA_JV_RFCOMM_START *p_start, uint32_t id) {
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (!slot)
 goto out;

 if (p_start->status == BTA_JV_SUCCESS) {
    slot->rfc_handle = p_start->handle;
 } else
    cleanup_rfc_slot(slot);

out:;
  pthread_mutex_unlock(&slot_lock);
}
