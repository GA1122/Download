static void btif_hl_ctrl_cback(tBTA_HL_CTRL_EVT event, tBTA_HL_CTRL *p_data){
 bt_status_t status;
 int param_len = 0;

    BTIF_TRACE_DEBUG("%s event %d", __FUNCTION__, event);
    btif_hl_display_calling_process_name();

 switch ( event )
 {
 case BTA_HL_CTRL_ENABLE_CFM_EVT:
 case BTA_HL_CTRL_DISABLE_CFM_EVT:
            param_len = sizeof(tBTA_HL_CTRL_ENABLE_DISABLE);
 break;
 default:
 break;
 }

    status = btif_transfer_context(btif_hl_upstreams_ctrl_evt, (uint16_t)event, (void*)p_data, param_len, NULL);
    ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
}
