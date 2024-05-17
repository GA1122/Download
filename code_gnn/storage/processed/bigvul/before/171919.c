void btif_hl_send_destroyed_cb(btif_hl_app_cb_t *p_acb ){
 bt_bdaddr_t     bd_addr;
 int             app_id = (int) btif_hl_get_app_id(p_acb->delete_mdl.channel_id);

    btif_hl_copy_bda(&bd_addr, p_acb->delete_mdl.bd_addr);
    BTIF_TRACE_DEBUG("%s",__FUNCTION__);
    BTIF_TRACE_DEBUG("call channel state callback channel_id=0x%08x mdep_cfg_idx=%d, state=%d fd=%d",p_acb->delete_mdl.channel_id,
                      p_acb->delete_mdl.mdep_cfg_idx, BTHL_CONN_STATE_DESTROYED, 0);
    btif_hl_display_bt_bda(&bd_addr);

    BTIF_HL_CALL_CBACK(bt_hl_callbacks, channel_state_cb,  app_id,
 &bd_addr, p_acb->delete_mdl.mdep_cfg_idx,
                       p_acb->delete_mdl.channel_id, BTHL_CONN_STATE_DESTROYED, 0 );
}
