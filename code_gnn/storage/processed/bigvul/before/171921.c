void btif_hl_send_setup_connecting_cb(UINT8 app_idx, UINT8 mcl_idx){
 btif_hl_pending_chan_cb_t *p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);
 bt_bdaddr_t                 bd_addr;
 int                         app_id = (int) btif_hl_get_app_id(p_pcb->channel_id);

    btif_hl_copy_bda(&bd_addr, p_pcb->bd_addr);

 if (p_pcb->in_use && p_pcb->cb_state == BTIF_HL_CHAN_CB_STATE_CONNECTING_PENDING)
 {
        BTIF_TRACE_DEBUG("%s",__FUNCTION__);
        BTIF_TRACE_DEBUG("call channel state callback  channel_id=0x%08x mdep_cfg_idx=%d state=%d fd=%d",p_pcb->channel_id,
                          p_pcb->mdep_cfg_idx, BTHL_CONN_STATE_CONNECTING, 0);
        btif_hl_display_bt_bda(&bd_addr);

        BTIF_HL_CALL_CBACK(bt_hl_callbacks, channel_state_cb, app_id,
 &bd_addr, p_pcb->mdep_cfg_idx,
                           p_pcb->channel_id, BTHL_CONN_STATE_CONNECTING, 0 );
        btif_hl_set_chan_cb_state(app_idx, mcl_idx, BTIF_HL_CHAN_CB_STATE_CONNECTED_PENDING);
 }
}
