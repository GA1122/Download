static rfc_slot_t *create_srv_accept_rfc_slot(rfc_slot_t *srv_rs, const bt_bdaddr_t *addr, int open_handle, int new_listen_handle) {
 rfc_slot_t *accept_rs = alloc_rfc_slot(addr, srv_rs->service_name, srv_rs->service_uuid, srv_rs->scn, 0, false);
 if (!accept_rs) {
    LOG_ERROR("%s unable to allocate RFCOMM slot.", __func__);
 return NULL;
 }

  accept_rs->f.server = false;
  accept_rs->f.connected = true;
  accept_rs->security = srv_rs->security;
  accept_rs->mtu = srv_rs->mtu;
  accept_rs->role = srv_rs->role;
  accept_rs->rfc_handle = open_handle;
  accept_rs->rfc_port_handle = BTA_JvRfcommGetPortHdl(open_handle);

  srv_rs->rfc_handle = new_listen_handle;
  srv_rs->rfc_port_handle = BTA_JvRfcommGetPortHdl(new_listen_handle);

  assert(accept_rs->rfc_port_handle != srv_rs->rfc_port_handle);

 uint32_t new_listen_id = accept_rs->id;
  accept_rs->id = srv_rs->id;
  srv_rs->id = new_listen_id;

 return accept_rs;
}
