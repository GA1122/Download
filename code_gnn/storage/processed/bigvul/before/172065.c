static uint32_t on_srv_rfc_connect(tBTA_JV_RFCOMM_SRV_OPEN *p_open, uint32_t id) {
 uint32_t new_listen_slot_id = 0;
  pthread_mutex_lock(&slot_lock);

 rfc_slot_t *srv_rs = find_rfc_slot_by_id(id);
 if (!srv_rs)
 goto out;

 rfc_slot_t *accept_rs = create_srv_accept_rfc_slot(srv_rs, (const bt_bdaddr_t *)p_open->rem_bda, p_open->handle, p_open->new_listen_handle);
 if (!accept_rs)
 goto out;

  btsock_thread_add_fd(pth, srv_rs->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_EXCEPTION, srv_rs->id);
  btsock_thread_add_fd(pth, accept_rs->fd, BTSOCK_RFCOMM, SOCK_THREAD_FD_RD, accept_rs->id);
  send_app_connect_signal(srv_rs->fd, &accept_rs->addr, srv_rs->scn, 0, accept_rs->app_fd);
  accept_rs->app_fd = INVALID_FD;  
  new_listen_slot_id = srv_rs->id;

out:;
  pthread_mutex_unlock(&slot_lock);
 return new_listen_slot_id;
}
