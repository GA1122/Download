static void on_cl_rfc_init(tBTA_JV_RFCOMM_CL_INIT *p_init, uint32_t id) {
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (!slot)
 goto out;

 if (p_init->status == BTA_JV_SUCCESS)
    slot->rfc_handle = p_init->handle;
 else
    cleanup_rfc_slot(slot);

out:;
  pthread_mutex_unlock(&slot_lock);
}
