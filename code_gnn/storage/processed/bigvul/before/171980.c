static void cleanup_ctrl()
{
    BTIF_TRACE_EVENT("## %s ##", __FUNCTION__);

 if (bt_rc_ctrl_callbacks)
 {
        bt_rc_ctrl_callbacks = NULL;
 }
    memset(&btif_rc_cb, 0, sizeof(btif_rc_cb_t));
    lbl_destroy();
    BTIF_TRACE_EVENT("## %s ## completed", __FUNCTION__);
}
