void handle_rc_passthrough_cmd ( tBTA_AV_REMOTE_CMD *p_remote_cmd)
{
 const char *status;
 int pressed, i;

    BTIF_TRACE_DEBUG("%s: p_remote_cmd->rc_id=%d", __FUNCTION__, p_remote_cmd->rc_id);

  
 if (p_remote_cmd)
 {
  
 if ((p_remote_cmd->rc_id == BTA_AV_RC_PLAY) && (!btif_av_is_connected()))
 {
 if (p_remote_cmd->key_state == AVRC_STATE_PRESS)
 {
                APPL_TRACE_WARNING("%s: AVDT not open, queuing the PLAY command", __FUNCTION__);
                btif_rc_cb.rc_pending_play = TRUE;
 }
 return;
 }

 if ((p_remote_cmd->rc_id == BTA_AV_RC_PAUSE) && (btif_rc_cb.rc_pending_play))
 {
            APPL_TRACE_WARNING("%s: Clear the pending PLAY on PAUSE received", __FUNCTION__);
            btif_rc_cb.rc_pending_play = FALSE;
 return;
 }
 }

 if ((p_remote_cmd->rc_id == BTA_AV_RC_STOP) && (!btif_av_stream_started_ready()))
 {
        APPL_TRACE_WARNING("%s: Stream suspended, ignore STOP cmd",__FUNCTION__);
 return;
 }

 if (p_remote_cmd->key_state == AVRC_STATE_RELEASE) {
        status = "released";
        pressed = 0;
 } else {
        status = "pressed";
        pressed = 1;
 }

  
 if((p_remote_cmd->rc_id == BTA_AV_RC_PLAY || p_remote_cmd->rc_id == BTA_AV_RC_PAUSE)&&
 (btif_hf_call_terminated_recently() == TRUE) &&
 (check_cod( (const bt_bdaddr_t*)&(btif_rc_cb.rc_addr), COD_AV_HEADSETS) != TRUE))
 {
        BTIF_TRACE_DEBUG("%s:Dropping the play/Pause command received right after call end cmd:%d",
                           __FUNCTION__,p_remote_cmd->rc_id);
 return;
 }

 if (p_remote_cmd->rc_id == BTA_AV_RC_FAST_FOR || p_remote_cmd->rc_id == BTA_AV_RC_REWIND) {
        HAL_CBACK(bt_rc_callbacks, passthrough_cmd_cb, p_remote_cmd->rc_id, pressed);
 return;
 }

 for (i = 0; key_map[i].name != NULL; i++) {
 if (p_remote_cmd->rc_id == key_map[i].avrcp) {
            BTIF_TRACE_DEBUG("%s: %s %s", __FUNCTION__, key_map[i].name, status);

  
 if ((key_map[i].release_quirk == 1) && (pressed == 0))
 {
                BTIF_TRACE_DEBUG("%s: AVRC %s Release Faked earlier, drowned now",
                                  __FUNCTION__, key_map[i].name);
 return;
 }
            send_key(uinput_fd, key_map[i].mapped_id, pressed);
 if ((key_map[i].release_quirk == 1) && (pressed == 1))
 {
                GKI_delay(30);  
                BTIF_TRACE_DEBUG("%s: AVRC %s Release quirk enabled, send release now",
                                  __FUNCTION__, key_map[i].name);
                send_key(uinput_fd, key_map[i].mapped_id, 0);
 }
 break;
 }
 }

 if (key_map[i].name == NULL)
        BTIF_TRACE_ERROR("%s AVRCP: unknown button 0x%02X %s", __FUNCTION__,
                        p_remote_cmd->rc_id, status);
}
