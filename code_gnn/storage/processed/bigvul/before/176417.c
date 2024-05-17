static bt_status_t disconnect(RawAddress* bd_addr) {
  BTIF_TRACE_EVENT("%s", __func__);
  CHECK_BTAV_INIT();

  
 return btif_transfer_context(btif_av_handle_event, BTIF_AV_DISCONNECT_REQ_EVT,
 (char*)bd_addr, sizeof(RawAddress), NULL);
}
