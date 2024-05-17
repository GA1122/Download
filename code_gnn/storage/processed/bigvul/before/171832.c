static void btif_hl_cback(tBTA_HL_EVT event, tBTA_HL *p_data){
 bt_status_t status;
 int param_len = 0;
    BTIF_TRACE_DEBUG("%s event %d", __FUNCTION__, event);
    btif_hl_display_calling_process_name();
 switch (event)
 {
 case BTA_HL_REGISTER_CFM_EVT:
            param_len = sizeof(tBTA_HL_REGISTER_CFM);
 break;
 case BTA_HL_SDP_INFO_IND_EVT:
            param_len = sizeof(tBTA_HL_SDP_INFO_IND);
 break;
 case BTA_HL_DEREGISTER_CFM_EVT:
            param_len = sizeof(tBTA_HL_DEREGISTER_CFM);
 break;
 case BTA_HL_SDP_QUERY_CFM_EVT:
            param_len = sizeof(tBTA_HL_SDP_QUERY_CFM);
 break;
 case BTA_HL_CCH_OPEN_CFM_EVT:
            param_len = sizeof(tBTA_HL_CCH_OPEN_CFM);
 break;
 case BTA_HL_DCH_OPEN_CFM_EVT:
            param_len = sizeof(tBTA_HL_DCH_OPEN_CFM);
 break;
 case BTA_HL_CCH_OPEN_IND_EVT:
            param_len = sizeof(tBTA_HL_CCH_OPEN_IND);
 break;
 case BTA_HL_DCH_CREATE_IND_EVT:
            param_len = sizeof(tBTA_HL_DCH_CREATE_IND);
 break;
 case BTA_HL_DCH_OPEN_IND_EVT:
            param_len = sizeof(tBTA_HL_DCH_OPEN_IND);
 break;
 case BTA_HL_DELETE_MDL_IND_EVT:
            param_len = sizeof(tBTA_HL_MDL_IND);
 break;
 case BTA_HL_DELETE_MDL_CFM_EVT:
            param_len = sizeof(tBTA_HL_MDL_CFM);
 break;
 case BTA_HL_DCH_RECONNECT_CFM_EVT:
            param_len = sizeof(tBTA_HL_DCH_OPEN_CFM);
 break;
 case BTA_HL_CCH_CLOSE_CFM_EVT:
            param_len = sizeof(tBTA_HL_MCL_CFM);
 break;
 case BTA_HL_CCH_CLOSE_IND_EVT:
            param_len = sizeof(tBTA_HL_CCH_CLOSE_IND);
 break;
 case BTA_HL_DCH_CLOSE_IND_EVT:
            param_len = sizeof(tBTA_HL_DCH_CLOSE_IND);
 break;
 case BTA_HL_DCH_CLOSE_CFM_EVT:
            param_len = sizeof(tBTA_HL_MDL_CFM);
 break;
 case BTA_HL_DCH_ECHO_TEST_CFM_EVT:
            param_len = sizeof(tBTA_HL_MCL_CFM);
 break;
 case BTA_HL_DCH_RECONNECT_IND_EVT:
            param_len = sizeof(tBTA_HL_DCH_OPEN_IND);
 break;
 case BTA_HL_CONG_CHG_IND_EVT:
            param_len = sizeof(tBTA_HL_DCH_CONG_IND);
 break;
 case BTA_HL_DCH_ABORT_IND_EVT:
            param_len = sizeof(tBTA_HL_MCL_IND);
 break;
 case BTA_HL_DCH_ABORT_CFM_EVT:
            param_len = sizeof(tBTA_HL_MCL_CFM);
 break;
 case BTA_HL_DCH_SEND_DATA_CFM_EVT:
            param_len = sizeof(tBTA_HL_MDL_CFM);
 break;
 case BTA_HL_DCH_RCV_DATA_IND_EVT:
            param_len = sizeof(tBTA_HL_MDL_IND);
 break;
 default:
            param_len = sizeof(tBTA_HL_MDL_IND);
 break;
 }
    status = btif_transfer_context(btif_hl_upstreams_evt, (uint16_t)event, (void*)p_data, param_len, NULL);

  
    ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}
