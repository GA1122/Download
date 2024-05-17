static void sdp_snd_service_search_req(tCONN_CB* p_ccb, uint8_t cont_len,
 uint8_t* p_cont) {
 uint8_t *p, *p_start, *p_param_len;
  BT_HDR* p_cmd = (BT_HDR*)osi_malloc(SDP_DATA_BUF_SIZE);
 uint16_t param_len;

  
  p_cmd->offset = L2CAP_MIN_OFFSET;
  p = p_start = (uint8_t*)(p_cmd + 1) + L2CAP_MIN_OFFSET;

  
  UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_SEARCH_REQ);
  UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
  p_ccb->transaction_id++;

  
  p_param_len = p;
  p += 2;

 
#if (SDP_BROWSE_PLUS == TRUE)
  p = sdpu_build_uuid_seq(p, 1,
 &p_ccb->p_db->uuid_filters[p_ccb->cur_uuid_idx]);
#else
  p = sdpu_build_uuid_seq(p, p_ccb->p_db->num_uuid_filters,
                          p_ccb->p_db->uuid_filters);
#endif

  
  UINT16_TO_BE_STREAM(p, sdp_cb.max_recs_per_search);

  
  UINT8_TO_BE_STREAM(p, cont_len);

  
 if (cont_len && p_cont) {
    memcpy(p, p_cont, cont_len);
    p += cont_len;
 }

  
  param_len = (uint16_t)(p - p_param_len - 2);
  UINT16_TO_BE_STREAM(p_param_len, param_len);

  p_ccb->disc_state = SDP_DISC_WAIT_HANDLES;

  
  p_cmd->len = (uint16_t)(p - p_start);

#if (SDP_DEBUG_RAW == TRUE)
  SDP_TRACE_WARNING("sdp_snd_service_search_req cont_len :%d disc_state:%d",
                    cont_len, p_ccb->disc_state);
#endif

  L2CA_DataWrite(p_ccb->connection_id, p_cmd);

  
  alarm_set_on_mloop(p_ccb->sdp_conn_timer, SDP_INACT_TIMEOUT_MS,
                     sdp_conn_timer_timeout, p_ccb);
}
