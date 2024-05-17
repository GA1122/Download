static void btif_hl_proc_cb_evt(UINT16 event, char* p_param){

 btif_hl_evt_cb_t *p_data = (btif_hl_evt_cb_t *)p_param;
 bt_bdaddr_t                     bd_addr;
 bthl_channel_state_t            state=BTHL_CONN_STATE_DISCONNECTED;
    BOOLEAN                         send_chan_cb=TRUE;
    tBTA_HL_REG_PARAM               reg_param;
 btif_hl_app_cb_t *p_acb;
 bthl_app_reg_state_t            reg_state = BTHL_APP_REG_STATE_REG_FAILED;
    UINT8                           preg_idx;
 bt_status_t                     bt_status;

    BTIF_TRACE_DEBUG("%s event %d", __FUNCTION__, event);
    btif_hl_display_calling_process_name();

 switch (event)
 {
 case BTIF_HL_SEND_CONNECTED_CB:
 case BTIF_HL_SEND_DISCONNECTED_CB:
 if (p_data->chan_cb.cb_state == BTIF_HL_CHAN_CB_STATE_CONNECTED_PENDING)
                state = BTHL_CONN_STATE_CONNECTED;
 else if (p_data->chan_cb.cb_state == BTIF_HL_CHAN_CB_STATE_DISCONNECTED_PENDING)
                state = BTHL_CONN_STATE_DISCONNECTED;
 else
                send_chan_cb = FALSE;

 if (send_chan_cb)
 {
                btif_hl_copy_bda(&bd_addr, p_data->chan_cb.bd_addr);
                BTIF_TRACE_DEBUG("state callbk: ch_id=0x%08x cb_state=%d state=%d  fd=%d",
                                  p_data->chan_cb.channel_id,
                                  p_data->chan_cb.cb_state,
                                  state,  p_data->chan_cb.fd);
                btif_hl_display_bt_bda(&bd_addr);
                BTIF_HL_CALL_CBACK(bt_hl_callbacks, channel_state_cb,  p_data->chan_cb.app_id,
 &bd_addr, p_data->chan_cb.mdep_cfg_index,
                                   p_data->chan_cb.channel_id, state, p_data->chan_cb.fd );
 }

 break;
 case BTIF_HL_REG_APP:
            p_acb  = BTIF_HL_GET_APP_CB_PTR(p_data->reg.app_idx);
            BTIF_TRACE_DEBUG("Rcv BTIF_HL_REG_APP app_idx=%d reg_pending=%d", p_data->reg.app_idx, p_acb->reg_pending);
 if (btif_hl_get_state() == BTIF_HL_STATE_ENABLED && p_acb->reg_pending)
 {
                BTIF_TRACE_DEBUG("Rcv BTIF_HL_REG_APP reg_counter=%d",reg_counter);
                p_acb->reg_pending = FALSE;
                reg_param.dev_type = p_acb->dev_type;
                reg_param.sec_mask = BTA_SEC_AUTHENTICATE | BTA_SEC_ENCRYPT;
                reg_param.p_srv_name = p_acb->srv_name;
                reg_param.p_srv_desp = p_acb->srv_desp;
                reg_param.p_provider_name = p_acb->provider_name;
                btif_hl_proc_reg_request (p_data->reg.app_idx, p_acb->app_id, &reg_param, btif_hl_cback);
 }
 else
 {
                BTIF_TRACE_DEBUG("reg request is processed state=%d reg_pending=%d", btif_hl_get_state(), p_acb->reg_pending);
 }

 break;

 case BTIF_HL_UNREG_APP:
            BTIF_TRACE_DEBUG("Rcv BTIF_HL_UNREG_APP app_idx=%d", p_data->unreg.app_idx );
            p_acb = BTIF_HL_GET_APP_CB_PTR(p_data->unreg.app_idx);
 if (btif_hl_get_state() == BTIF_HL_STATE_ENABLED)
 {
 if(reg_counter >= 1)
                    BTA_HlUpdate(p_acb->app_id,NULL,FALSE,NULL);
 else
                    BTA_HlDeregister(p_acb->app_id, p_acb->app_handle);
 }
 break;
 case BTIF_HL_UPDATE_MDL:
            BTIF_TRACE_DEBUG("Rcv BTIF_HL_UPDATE_MDL app_idx=%d", p_data->update_mdl.app_idx );
            p_acb = BTIF_HL_GET_APP_CB_PTR(p_data->update_mdl.app_idx);
 break;

 default:
            BTIF_TRACE_ERROR("Unknown event %d", event);
 break;
 }
}
