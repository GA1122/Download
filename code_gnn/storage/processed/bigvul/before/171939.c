static bt_status_t init( bthl_callbacks_t* callbacks ){
 bt_status_t status = BT_STATUS_SUCCESS;

    BTIF_TRACE_EVENT("%s", __FUNCTION__);
    btif_hl_display_calling_process_name();
    bt_hl_callbacks_cb = *callbacks;
    bt_hl_callbacks = &bt_hl_callbacks_cb;
    btif_hl_soc_thread_init();
    reg_counter = 0;
 return status;
}
