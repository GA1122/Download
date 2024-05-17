static void cleanup()
{
    BTIF_TRACE_EVENT("## %s ##", __FUNCTION__);
    close_uinput();
 if (bt_rc_callbacks)
 {
        bt_rc_callbacks = NULL;
 }
    memset(&btif_rc_cb, 0, sizeof(btif_rc_cb_t));
    lbl_destroy();
    BTIF_TRACE_EVENT("## %s ## completed", __FUNCTION__);
}
