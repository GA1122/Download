static rfc_slot_t *alloc_rfc_slot(const bt_bdaddr_t *addr, const char *name, const uint8_t *uuid, int channel, int flags, bool server) {
 int security = 0;
 if(flags & BTSOCK_FLAG_ENCRYPT)
      security |= server ? BTM_SEC_IN_ENCRYPT : BTM_SEC_OUT_ENCRYPT;
 if(flags & BTSOCK_FLAG_AUTH)
      security |= server ? BTM_SEC_IN_AUTHENTICATE : BTM_SEC_OUT_AUTHENTICATE;
 if(flags & BTSOCK_FLAG_AUTH_MITM)
      security |= server ? BTM_SEC_IN_MITM : BTM_SEC_OUT_MITM;
 if(flags & BTSOCK_FLAG_AUTH_16_DIGIT)
      security |= BTM_SEC_IN_MIN_16_DIGIT_PIN;

 rfc_slot_t *slot = find_free_slot();
 if (!slot) {
    LOG_ERROR("%s unable to find free RFCOMM slot.", __func__);
 return NULL;
 }

 int fds[2] = { INVALID_FD, INVALID_FD };
 if (socketpair(AF_LOCAL, SOCK_STREAM, 0, fds) == -1) {
    LOG_ERROR("%s error creating socketpair: %s", __func__, strerror(errno));
 return NULL;
 }

 if (++rfc_slot_id == 0)
    rfc_slot_id = 1;

  slot->fd = fds[0];
  slot->app_fd = fds[1];
  slot->security = security;
  slot->scn = channel;

 if(!is_uuid_empty(uuid)) {
    memcpy(slot->service_uuid, uuid, sizeof(slot->service_uuid));
    slot->is_service_uuid_valid = true;
 } else {
    memset(slot->service_uuid, 0, sizeof(slot->service_uuid));
    slot->is_service_uuid_valid = false;
 }
 if(name && *name) {
    strlcpy(slot->service_name, name, sizeof(slot->service_name));
 } else {
    memset(slot->service_name, 0, sizeof(slot->service_name));
 }
 if (addr)
    slot->addr = *addr;

  slot->id = rfc_slot_id;
  slot->f.server = server;

 return slot;
}
