static bt_status_t init_ctrl(btrc_ctrl_callbacks_t* callbacks )
{
    BTIF_TRACE_EVENT("## %s ##", __FUNCTION__);
 bt_status_t result = BT_STATUS_SUCCESS;

 if (bt_rc_ctrl_callbacks)
 return BT_STATUS_DONE;

    bt_rc_ctrl_callbacks = callbacks;
    memset (&btif_rc_cb, 0, sizeof(btif_rc_cb));
    btif_rc_cb.rc_vol_label=MAX_LABEL;
    btif_rc_cb.rc_volume=MAX_VOLUME;
    lbl_init();

 return result;
}
