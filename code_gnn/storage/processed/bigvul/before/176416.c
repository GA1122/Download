static bt_status_t connect_int(RawAddress* bd_addr, uint16_t uuid) {
 btif_av_connect_req_t connect_req;
  connect_req.target_bda = bd_addr;
  connect_req.uuid = uuid;
  BTIF_TRACE_EVENT("%s", __func__);

  btif_sm_dispatch(btif_av_cb.sm_handle, BTIF_AV_CONNECT_REQ_EVT,
 (char*)&connect_req);

 return BT_STATUS_SUCCESS;
}
