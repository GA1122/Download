void btif_hl_send_disconnecting_cb(UINT8 app_idx, UINT8 mcl_idx, UINT8 mdl_idx){
 btif_hl_mdl_cb_t *p_dcb = BTIF_HL_GET_MDL_CB_PTR( app_idx,  mcl_idx, mdl_idx);
 btif_hl_soc_cb_t *p_scb = p_dcb->p_scb;
 bt_bdaddr_t             bd_addr;
 int                     app_id = (int) btif_hl_get_app_id(p_scb->channel_id);

    btif_hl_copy_bda(&bd_addr, p_scb->bd_addr);

    BTIF_TRACE_DEBUG("%s",__FUNCTION__);
    BTIF_TRACE_DEBUG("call channel state callback  channel_id=0x%08x mdep_cfg_idx=%d, state=%d fd=%d",p_scb->channel_id,
                      p_scb->mdep_cfg_idx, BTHL_CONN_STATE_DISCONNECTING, p_scb->socket_id[0]);
    btif_hl_display_bt_bda(&bd_addr);
    BTIF_HL_CALL_CBACK(bt_hl_callbacks, channel_state_cb,  app_id,
 &bd_addr, p_scb->mdep_cfg_idx,
                       p_scb->channel_id, BTHL_CONN_STATE_DISCONNECTING, p_scb->socket_id[0] );
}
