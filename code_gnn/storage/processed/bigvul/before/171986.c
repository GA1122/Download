void handle_rc_disconnect (tBTA_AV_RC_CLOSE *p_rc_close)
{
#if (AVRC_CTLR_INCLUDED == TRUE)
 bt_bdaddr_t rc_addr;
    tBTA_AV_FEAT features;
#endif
    BTIF_TRACE_DEBUG("%s: rc_handle: %d", __FUNCTION__, p_rc_close->rc_handle);
 if ((p_rc_close->rc_handle != btif_rc_cb.rc_handle)
 && (bdcmp(btif_rc_cb.rc_addr, p_rc_close->peer_addr)))
 {
        BTIF_TRACE_ERROR("Got disconnect of unknown device");
 return;
 }

    btif_rc_cb.rc_handle = 0;
    btif_rc_cb.rc_connected = FALSE;
    memset(btif_rc_cb.rc_addr, 0, sizeof(BD_ADDR));
    memset(btif_rc_cb.rc_notif, 0, sizeof(btif_rc_cb.rc_notif));
#if (AVRC_CTLR_INCLUDED == TRUE)
    features = btif_rc_cb.rc_features;
#endif
    btif_rc_cb.rc_features = 0;
    btif_rc_cb.rc_vol_label=MAX_LABEL;
    btif_rc_cb.rc_volume=MAX_VOLUME;
    init_all_transactions();
    close_uinput();
#if (AVRC_CTLR_INCLUDED == TRUE)
    bdcpy(rc_addr.address, btif_rc_cb.rc_addr);
#endif
    memset(btif_rc_cb.rc_addr, 0, sizeof(BD_ADDR));
#if (AVRC_CTLR_INCLUDED == TRUE)
  
 if (features & BTA_AV_FEAT_RCTG) {
 if (bt_rc_ctrl_callbacks != NULL) {
            HAL_CBACK(bt_rc_ctrl_callbacks, connection_state_cb, FALSE, &rc_addr);
 }
 }
#endif
}
