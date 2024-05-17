static void btif_hl_upstreams_ctrl_evt(UINT16 event, char* p_param){
    tBTA_HL_CTRL *p_data = (tBTA_HL_CTRL *) p_param;
    UINT8               i;
    tBTA_HL_REG_PARAM   reg_param;
 btif_hl_app_cb_t *p_acb;

    BTIF_TRACE_DEBUG("%s event %d", __FUNCTION__, event);
    btif_hl_display_calling_process_name();

 switch ( event )
 {
 case BTA_HL_CTRL_ENABLE_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CTRL_ENABLE_CFM_EVT");
            BTIF_TRACE_DEBUG("status=%d", p_data->enable_cfm.status);

 if (p_data->enable_cfm.status == BTA_HL_STATUS_OK)
 {
                btif_hl_set_state(BTIF_HL_STATE_ENABLED);


 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
                    p_acb = BTIF_HL_GET_APP_CB_PTR(i);
 if (p_acb->in_use && p_acb->reg_pending)
 {
                        p_acb->reg_pending = FALSE;
                        reg_param.dev_type = p_acb->dev_type;
                        reg_param.sec_mask = BTA_SEC_AUTHENTICATE | BTA_SEC_ENCRYPT;
                        reg_param.p_srv_name = p_acb->srv_name;
                        reg_param.p_srv_desp = p_acb->srv_desp;
                        reg_param.p_provider_name = p_acb->provider_name;

                        BTIF_TRACE_DEBUG("Register pending app_id=%d", p_acb->app_id);
                        btif_hl_proc_reg_request (i, p_acb->app_id, &reg_param, btif_hl_cback);
 }
 }
 }

 break;
 case BTA_HL_CTRL_DISABLE_CFM_EVT:
            BTIF_TRACE_DEBUG("Rcv BTA_HL_CTRL_DISABLE_CFM_EVT");
            BTIF_TRACE_DEBUG("status=%d",
                              p_data->disable_cfm.status);

 if (p_data->disable_cfm.status == BTA_HL_STATUS_OK)
 {
                memset(p_btif_hl_cb, 0, sizeof(btif_hl_cb_t));
                btif_hl_set_state(BTIF_HL_STATE_DISABLED);
 }

 break;
 default:
 break;
 }
}
