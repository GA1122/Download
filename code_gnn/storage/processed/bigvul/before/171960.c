static bt_status_t btpan_jni_init(const btpan_callbacks_t* callbacks)
{
    BTIF_TRACE_DEBUG("stack_initialized = %d, btpan_cb.enabled:%d", stack_initialized, btpan_cb.enabled);
    callback = *callbacks;
    jni_initialized = TRUE;
 if (stack_initialized && !btpan_cb.enabled)
        btif_pan_init();
 return BT_STATUS_SUCCESS;
}
