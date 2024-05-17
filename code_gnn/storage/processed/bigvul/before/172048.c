bt_status_t btsock_rfc_listen(const char *service_name, const uint8_t *service_uuid, int channel, int *sock_fd, int flags) {
  assert(sock_fd != NULL);
  assert((service_uuid != NULL)
 || (channel >= 1 && channel <= MAX_RFC_CHANNEL)
 || ((flags & BTSOCK_FLAG_NO_SDP) != 0));

 *sock_fd = INVALID_FD;

 if (!is_init_done())
 return BT_STATUS_NOT_READY;

 if((flags & BTSOCK_FLAG_NO_SDP) == 0) {
 if(is_uuid_empty(service_uuid)) {
      APPL_TRACE_DEBUG("BTA_JvGetChannelId: service_uuid not set AND "
 "BTSOCK_FLAG_NO_SDP is not set - changing to SPP");
      service_uuid = UUID_SPP;  
 } else {
 int reserved_channel = get_reserved_rfc_channel(service_uuid);
 if (reserved_channel > 0) {
            channel = reserved_channel;
 }
 }
 }

 int status = BT_STATUS_FAIL;
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *slot = alloc_rfc_slot(NULL, service_name, service_uuid, channel, flags, true);
 if (!slot) {
    LOG_ERROR("%s unable to allocate RFCOMM slot.", __func__);
 goto out;
 }
  APPL_TRACE_DEBUG("BTA_JvGetChannelId: service_name: %s - channel: %d", service_name, channel);
  BTA_JvGetChannelId(BTA_JV_CONN_TYPE_RFCOMM, (void*) slot->id, channel);
 *sock_fd = slot->app_fd;  
  
  slot->app_fd = INVALID_FD;  
  btsock_thread_add_fd(pth, slot->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_EXCEPTION, slot->id);

  status = BT_STATUS_SUCCESS;

out:;
  pthread_mutex_unlock(&slot_lock);
 return status;
}
