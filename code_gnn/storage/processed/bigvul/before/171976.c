void btif_rc_handler(tBTA_AV_EVT event, tBTA_AV *p_data)
{
    BTIF_TRACE_DEBUG ("%s event:%s", __FUNCTION__, dump_rc_event(event));
 switch (event)
 {
 case BTA_AV_RC_OPEN_EVT:
 {
            BTIF_TRACE_DEBUG("Peer_features:%x", p_data->rc_open.peer_features);
            handle_rc_connect( &(p_data->rc_open) );
 }break;

 case BTA_AV_RC_CLOSE_EVT:
 {
            handle_rc_disconnect( &(p_data->rc_close) );
 }break;

 case BTA_AV_REMOTE_CMD_EVT:
 {
            BTIF_TRACE_DEBUG("rc_id:0x%x key_state:%d", p_data->remote_cmd.rc_id,
                               p_data->remote_cmd.key_state);
            handle_rc_passthrough_cmd( (&p_data->remote_cmd) );
 }
 break;
#if (AVRC_CTLR_INCLUDED == TRUE)
 case BTA_AV_REMOTE_RSP_EVT:
 {
            BTIF_TRACE_DEBUG("RSP: rc_id:0x%x key_state:%d", p_data->remote_rsp.rc_id,
                               p_data->remote_rsp.key_state);
            handle_rc_passthrough_rsp( (&p_data->remote_rsp) );
 }
 break;
#endif
 case BTA_AV_RC_FEAT_EVT:
 {
            BTIF_TRACE_DEBUG("Peer_features:%x", p_data->rc_feat.peer_features);
            btif_rc_cb.rc_features = p_data->rc_feat.peer_features;
            handle_rc_features();
 }
 break;
 case BTA_AV_META_MSG_EVT:
 {
            BTIF_TRACE_DEBUG("BTA_AV_META_MSG_EVT  code:%d label:%d", p_data->meta_msg.code,
                p_data->meta_msg.label);
            BTIF_TRACE_DEBUG("  company_id:0x%x len:%d handle:%d", p_data->meta_msg.company_id,
                p_data->meta_msg.len, p_data->meta_msg.rc_handle);
  
            handle_rc_metamsg_cmd(&(p_data->meta_msg));
 }
 break;
 default:
            BTIF_TRACE_DEBUG("Unhandled RC event : 0x%x", event);
 }
}
