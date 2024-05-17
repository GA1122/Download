static void btif_initiate_av_open_timer_timeout(UNUSED_ATTR void* data) {
 RawAddress peer_addr;
 btif_av_connect_req_t connect_req;

  
 if (btif_rc_get_connected_peer(&peer_addr)) {
    BTIF_TRACE_DEBUG("%s: Issuing connect to the remote RC peer", __func__);
  
    connect_req.target_bda = &peer_addr;
 if (bt_av_sink_callbacks != NULL)
      connect_req.uuid = UUID_SERVCLASS_AUDIO_SINK;
 else if (bt_av_src_callbacks != NULL)
      connect_req.uuid = UUID_SERVCLASS_AUDIO_SOURCE;
    btif_dispatch_sm_event(BTIF_AV_CONNECT_REQ_EVT, (char*)&connect_req,
 sizeof(connect_req));
 } else {
    BTIF_TRACE_ERROR("%s: No connected RC peers", __func__);
 }
}
