static void cleanup_rfc_slot(rfc_slot_t *slot) {
 if (slot->fd != INVALID_FD) {
    shutdown(slot->fd, SHUT_RDWR);
    close(slot->fd);
    slot->fd = INVALID_FD;
 }

 if (slot->app_fd != INVALID_FD) {
    close(slot->app_fd);
    slot->app_fd = INVALID_FD;
 }

 if (slot->sdp_handle > 0) {
    del_rfc_sdp_rec(slot->sdp_handle);
    slot->sdp_handle = 0;
 }

 if (slot->rfc_handle && !slot->f.closing && !slot->f.server) {
    BTA_JvRfcommClose(slot->rfc_handle, (void *)(uintptr_t)slot->id);
    slot->rfc_handle = 0;
 }

  free_rfc_slot_scn(slot);
  list_clear(slot->incoming_queue);

  slot->rfc_port_handle = 0;
  memset(&slot->f, 0, sizeof(slot->f));
  slot->id = 0;
  slot->scn_notified = false;
}
