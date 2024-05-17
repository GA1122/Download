static void process_service_search_rsp(tCONN_CB* p_ccb, uint8_t* p_reply,
 uint8_t* p_reply_end) {
 uint16_t xx;
 uint16_t total, cur_handles, orig;
 uint8_t cont_len;

  
  p_reply += 4;
  BE_STREAM_TO_UINT16(total, p_reply);
  BE_STREAM_TO_UINT16(cur_handles, p_reply);

  orig = p_ccb->num_handles;
  p_ccb->num_handles += cur_handles;
 if (p_ccb->num_handles == 0) {
    SDP_TRACE_WARNING("SDP - Rcvd ServiceSearchRsp, no matches");
    sdp_disconnect(p_ccb, SDP_NO_RECS_MATCH);
 return;
 }

  
 if (total > sdp_cb.max_recs_per_search) total = sdp_cb.max_recs_per_search;
 if (p_ccb->num_handles > sdp_cb.max_recs_per_search)
    p_ccb->num_handles = sdp_cb.max_recs_per_search;

 for (xx = orig; xx < p_ccb->num_handles; xx++)
    BE_STREAM_TO_UINT32(p_ccb->handles[xx], p_reply);

  BE_STREAM_TO_UINT8(cont_len, p_reply);
 if (cont_len != 0) {
 if (cont_len > SDP_MAX_CONTINUATION_LEN) {
      sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
 return;
 }
 if (p_reply + cont_len > p_reply_end) {
      android_errorWriteLog(0x534e4554, "68161546");
      sdp_disconnect(p_ccb, SDP_INVALID_CONT_STATE);
 return;
 }
  
    sdp_snd_service_search_req(p_ccb, cont_len, p_reply);
 } else {
  
    p_ccb->disc_state = SDP_DISC_WAIT_ATTR;

  
    process_service_attr_rsp(p_ccb, NULL, NULL);
 }
}
