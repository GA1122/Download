void handle_rc_connect (tBTA_AV_RC_OPEN *p_rc_open)
{
    BTIF_TRACE_DEBUG("%s: rc_handle: %d", __FUNCTION__, p_rc_open->rc_handle);
 bt_status_t result = BT_STATUS_SUCCESS;
#if (AVRC_CTLR_INCLUDED == TRUE)
 bt_bdaddr_t rc_addr;
#endif

 if(p_rc_open->status == BTA_AV_SUCCESS)
 {
 if (btif_rc_cb.rc_connected)
 {
            BTIF_TRACE_ERROR("Got RC OPEN in connected state, Connected RC: %d \
                and Current RC: %d", btif_rc_cb.rc_handle,p_rc_open->rc_handle );
 if ((btif_rc_cb.rc_handle != p_rc_open->rc_handle)
 && (bdcmp(btif_rc_cb.rc_addr, p_rc_open->peer_addr)))
 {
                BTIF_TRACE_DEBUG("Got RC connected for some other handle");
                BTA_AvCloseRc(p_rc_open->rc_handle);
 return;
 }
 }
        memcpy(btif_rc_cb.rc_addr, p_rc_open->peer_addr, sizeof(BD_ADDR));
        btif_rc_cb.rc_features = p_rc_open->peer_features;
        btif_rc_cb.rc_vol_label=MAX_LABEL;
        btif_rc_cb.rc_volume=MAX_VOLUME;

        btif_rc_cb.rc_connected = TRUE;
        btif_rc_cb.rc_handle = p_rc_open->rc_handle;

  
 if (btif_rc_cb.rc_features != 0)
            handle_rc_features();

        result = uinput_driver_check();
 if(result == BT_STATUS_SUCCESS)
 {
            init_uinput();
 }
#if (AVRC_CTLR_INCLUDED == TRUE)
        bdcpy(rc_addr.address, btif_rc_cb.rc_addr);
  
 if (btif_rc_cb.rc_features & BTA_AV_FEAT_RCTG) {
 if (bt_rc_ctrl_callbacks != NULL) {
                HAL_CBACK(bt_rc_ctrl_callbacks, connection_state_cb, TRUE, &rc_addr);
 }
 }
#endif
 }
 else
 {
        BTIF_TRACE_ERROR("%s Connect failed with error code: %d",
            __FUNCTION__, p_rc_open->status);
        btif_rc_cb.rc_connected = FALSE;
 }
}
