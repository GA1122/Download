void sdp_disc_server_rsp(tCONN_CB* p_ccb, BT_HDR* p_msg) {
 uint8_t *p, rsp_pdu;
 bool invalid_pdu = true;

#if (SDP_DEBUG_RAW == TRUE)
  SDP_TRACE_WARNING("sdp_disc_server_rsp disc_state:%d", p_ccb->disc_state);
#endif

  
  alarm_cancel(p_ccb->sdp_conn_timer);

  
  p = (uint8_t*)(p_msg + 1) + p_msg->offset;
 uint8_t* p_end = p + p_msg->len;

  BE_STREAM_TO_UINT8(rsp_pdu, p);

  p_msg->len--;

 switch (rsp_pdu) {
 case SDP_PDU_SERVICE_SEARCH_RSP:
 if (p_ccb->disc_state == SDP_DISC_WAIT_HANDLES) {
        process_service_search_rsp(p_ccb, p, p_end);
        invalid_pdu = false;
 }
 break;

 case SDP_PDU_SERVICE_ATTR_RSP:
 if (p_ccb->disc_state == SDP_DISC_WAIT_ATTR) {
        process_service_attr_rsp(p_ccb, p, p_end);
        invalid_pdu = false;
 }
 break;

 case SDP_PDU_SERVICE_SEARCH_ATTR_RSP:
 if (p_ccb->disc_state == SDP_DISC_WAIT_SEARCH_ATTR) {
        process_service_search_attr_rsp(p_ccb, p, p_end);
        invalid_pdu = false;
 }
 break;
 }

 if (invalid_pdu) {
    SDP_TRACE_WARNING("SDP - Unexp. PDU: %d in state: %d", rsp_pdu,
                      p_ccb->disc_state);
    sdp_disconnect(p_ccb, SDP_GENERIC_ERROR);
 }
}
