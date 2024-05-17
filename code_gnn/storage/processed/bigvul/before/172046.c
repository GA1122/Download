bt_status_t btsock_rfc_connect(const bt_bdaddr_t *bd_addr, const uint8_t *service_uuid, int channel, int *sock_fd, int flags) {
  assert(sock_fd != NULL);
  assert(service_uuid != NULL || (channel >= 1 && channel <= MAX_RFC_CHANNEL));

 *sock_fd = INVALID_FD;

 if (!is_init_done())
 return BT_STATUS_NOT_READY;

 int status = BT_STATUS_FAIL;
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = alloc_rfc_slot(bd_addr, NULL, service_uuid, channel, flags, false);
 if (!slot) {
    LOG_ERROR("%s unable to allocate RFCOMM slot.", __func__);
 goto out;
 }

 if (is_uuid_empty(service_uuid)) {
    tBTA_JV_STATUS ret = BTA_JvRfcommConnect(slot->security, slot->role, slot->scn, slot->addr.address, rfcomm_cback, (void *)(uintptr_t)slot->id);
 if (ret != BTA_JV_SUCCESS) {
      LOG_ERROR("%s unable to initiate RFCOMM connection: %d", __func__, ret);
      cleanup_rfc_slot(slot);
 goto out;
 }

 if (!send_app_scn(slot)) {
      LOG_ERROR("%s unable to send channel number.", __func__);
      cleanup_rfc_slot(slot);
 goto out;
 }
 } else {
    tSDP_UUID sdp_uuid;
    sdp_uuid.len = 16;
    memcpy(sdp_uuid.uu.uuid128, service_uuid, sizeof(sdp_uuid.uu.uuid128));

 if (!is_requesting_sdp()) {
      BTA_JvStartDiscovery((uint8_t *)bd_addr->address, 1, &sdp_uuid, (void *)(uintptr_t)slot->id);
      slot->f.pending_sdp_request = false;
      slot->f.doing_sdp_request = true;
 } else {
      slot->f.pending_sdp_request = true;
      slot->f.doing_sdp_request = false;
 }
 }

 *sock_fd = slot->app_fd;  
  slot->app_fd = INVALID_FD;  
  btsock_thread_add_fd(pth, slot->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_RD, slot->id);
  status = BT_STATUS_SUCCESS;

out:;
  pthread_mutex_unlock(&slot_lock);
 return status;
}
