static void on_cli_rfc_connect(tBTA_JV_RFCOMM_OPEN *p_open, uint32_t id) {
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = find_rfc_slot_by_id(id);
 if (!slot)
 goto out;

 if (p_open->status != BTA_JV_SUCCESS) {
    cleanup_rfc_slot(slot);
 goto out;
 }

  slot->rfc_port_handle = BTA_JvRfcommGetPortHdl(p_open->handle);
  memcpy(slot->addr.address, p_open->rem_bda, 6);

 if (send_app_connect_signal(slot->fd, &slot->addr, slot->scn, 0, -1))
    slot->f.connected = true;
 else
    LOG_ERROR("%s unable to send connect completion signal to caller.", __func__);

out:;
  pthread_mutex_unlock(&slot_lock);
}
