bt_status_t btif_in_execute_service_request(tBTA_SERVICE_ID service_id,
                                                BOOLEAN b_enable)
{
    BTIF_TRACE_DEBUG("%s service_id: %d", __FUNCTION__, service_id);
  
 switch (service_id)
 {
 case BTA_HFP_SERVICE_ID:
 case BTA_HSP_SERVICE_ID:
 {
              btif_hf_execute_service(b_enable);
 }break;
 case BTA_A2DP_SOURCE_SERVICE_ID:
 {
              btif_av_execute_service(b_enable);
 }break;
 case BTA_A2DP_SINK_SERVICE_ID:
 {
            btif_av_sink_execute_service(b_enable);
 }break;
 case BTA_HID_SERVICE_ID:
 {
              btif_hh_execute_service(b_enable);
 }break;
 case BTA_HFP_HS_SERVICE_ID:
 {
             btif_hf_client_execute_service(b_enable);
 }break;
 case BTA_SDP_SERVICE_ID:
 {
             btif_sdp_execute_service(b_enable);
 }break;
 default:
              BTIF_TRACE_ERROR("%s: Unknown service being enabled", __FUNCTION__);
 return BT_STATUS_FAIL;
 }
 return BT_STATUS_SUCCESS;
}
