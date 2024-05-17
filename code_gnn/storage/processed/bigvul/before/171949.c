void btif_pan_init()
{
    BTIF_TRACE_DEBUG("jni_initialized = %d, btpan_cb.enabled:%d", jni_initialized, btpan_cb.enabled);
    stack_initialized = true;

 if (jni_initialized && !btpan_cb.enabled)
 {
        BTIF_TRACE_DEBUG("Enabling PAN....");
        memset(&btpan_cb, 0, sizeof(btpan_cb));
        btpan_cb.tap_fd = INVALID_FD;
        btpan_cb.flow = 1;
 for (int i = 0; i < MAX_PAN_CONNS; i++)
            btpan_cleanup_conn(&btpan_cb.conns[i]);
        BTA_PanEnable(bta_pan_callback);
        btpan_cb.enabled = 1;
        btpan_enable(BTPAN_LOCAL_ROLE);
 }
}
