static void process_service_attr_rsp(tCONN_CB* p_ccb, uint8_t* p_reply,
 uint8_t* p_reply_end) {
 uint8_t *p_start, *p_param_len;
 uint16_t param_len, list_byte_count;
 bool cont_request_needed = false;

#if (SDP_DEBUG_RAW == TRUE)
  SDP_TRACE_WARNING("process_service_attr_rsp raw inc:%d",
                    SDP_RAW_DATA_INCLUDED);
#endif
  
 if (p_reply) {
#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("ID & len: 0x%02x-%02x-%02x-%02x", p_reply[0], p_reply[1],
                      p_reply[2], p_reply[3]);
#endif
  
    p_reply += 4;

    BE_STREAM_TO_UINT16(list_byte_count, p_reply);
#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("list_byte_count:%d", list_byte_count);
#endif

  
 if ((p_ccb->list_len + list_byte_count) > SDP_MAX_LIST_BYTE_COUNT) {
      sdp_disconnect(p_ccb, SDP_INVALID_PDU_SIZE);
 return;
 }

#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("list_len: %d, list_byte_count: %d", p_ccb->list_len,
                      list_byte_count);
#endif
 if (p_ccb->rsp_list == NULL)
      p_ccb->rsp_list = (uint8_t*)osi_malloc(SDP_MAX_LIST_BYTE_COUNT);
    memcpy(&p_ccb->rsp_list[p_ccb->list_len], p_reply, list_byte_count);
    p_ccb->list_len += list_byte_count;
    p_reply += list_byte_count;
#if (SDP_DEBUG_RAW == TRUE)
    SDP_TRACE_WARNING("list_len: %d(attr_rsp)", p_ccb->list_len);

  
    SDP_TRACE_WARNING("*p_reply:%d(%d)", *p_reply, SDP_MAX_CONTINUATION_LEN);
#endif
 if (*p_reply) {
 if (*p_reply > SDP_MAX_CONTINUATION_LEN) {
        sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
 return;
 }
      cont_request_needed = true;
 } else {
#if (SDP_RAW_DATA_INCLUDED == TRUE)
      SDP_TRACE_WARNING("process_service_attr_rsp");
      sdp_copy_raw_data(p_ccb, false);
#endif

  
 if (!save_attr_seq(p_ccb, &p_ccb->rsp_list[0],
 &p_ccb->rsp_list[p_ccb->list_len])) {
        sdp_disconnect(p_ccb, SDP_DB_FULL);
 return;
 }
      p_ccb->list_len = 0;
      p_ccb->cur_handle++;
 }
 }

  
 if (p_ccb->cur_handle < p_ccb->num_handles) {
    BT_HDR* p_msg = (BT_HDR*)osi_malloc(SDP_DATA_BUF_SIZE);
 uint8_t* p;

    p_msg->offset = L2CAP_MIN_OFFSET;
    p = p_start = (uint8_t*)(p_msg + 1) + L2CAP_MIN_OFFSET;

  
    UINT8_TO_BE_STREAM(p, SDP_PDU_SERVICE_ATTR_REQ);
    UINT16_TO_BE_STREAM(p, p_ccb->transaction_id);
    p_ccb->transaction_id++;

  
    p_param_len = p;
    p += 2;

    UINT32_TO_BE_STREAM(p, p_ccb->handles[p_ccb->cur_handle]);

  
    UINT16_TO_BE_STREAM(p, sdp_cb.max_attr_list_size);

  
 if (p_ccb->p_db->num_attr_filters)
      p = sdpu_build_attrib_seq(p, p_ccb->p_db->attr_filters,
                                p_ccb->p_db->num_attr_filters);
 else
      p = sdpu_build_attrib_seq(p, NULL, 0);

  
 if (cont_request_needed) {
 if ((p_reply + *p_reply + 1) <= p_reply_end) {
        memcpy(p, p_reply, *p_reply + 1);
        p += *p_reply + 1;
 } else {
        android_errorWriteLog(0x534e4554, "68161546");
 }
 } else
      UINT8_TO_BE_STREAM(p, 0);

  
    param_len = (uint16_t)(p - p_param_len - 2);
    UINT16_TO_BE_STREAM(p_param_len, param_len);

  
    p_msg->len = (uint16_t)(p - p_start);

    L2CA_DataWrite(p_ccb->connection_id, p_msg);

  
    alarm_set_on_mloop(p_ccb->sdp_conn_timer, SDP_INACT_TIMEOUT_MS,
                       sdp_conn_timer_timeout, p_ccb);
 } else {
    sdp_disconnect(p_ccb, SDP_SUCCESS);
 return;
 }
}
