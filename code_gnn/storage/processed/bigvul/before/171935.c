static void  cleanup( void ){
    BTIF_TRACE_EVENT("%s", __FUNCTION__);
    btif_hl_display_calling_process_name();
 if (bt_hl_callbacks)
 {
        btif_disable_service(BTA_HDP_SERVICE_ID);
        bt_hl_callbacks = NULL;
        reg_counter = 0;
 }

    btif_hl_disable();
    btif_hl_close_select_thread();
}
