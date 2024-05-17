void handle_rc_passthrough_rsp ( tBTA_AV_REMOTE_RSP *p_remote_rsp)
{
#if (AVRC_CTLR_INCLUDED == TRUE)
 const char *status;
 if (btif_rc_cb.rc_features & BTA_AV_FEAT_RCTG)
 {
 int key_state;
 if (p_remote_rsp->key_state == AVRC_STATE_RELEASE)
 {
            status = "released";
            key_state = 1;
 }
 else
 {
            status = "pressed";
            key_state = 0;
 }

        BTIF_TRACE_DEBUG("%s: rc_id=%d status=%s", __FUNCTION__, p_remote_rsp->rc_id, status);

        release_transaction(p_remote_rsp->label);
 if (bt_rc_ctrl_callbacks != NULL) {
            HAL_CBACK(bt_rc_ctrl_callbacks, passthrough_rsp_cb, p_remote_rsp->rc_id, key_state);
 }
 }
 else
 {
        BTIF_TRACE_ERROR("%s DUT does not support AVRCP controller role", __FUNCTION__);
 }
#else
    BTIF_TRACE_ERROR("%s AVRCP controller role is not enabled", __FUNCTION__);
#endif
}
