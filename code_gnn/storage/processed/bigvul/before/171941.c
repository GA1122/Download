static bt_status_t unregister_application(int app_id){
    UINT8               app_idx;
 int                 len;
 bt_status_t         status = BT_STATUS_SUCCESS;
 btif_hl_evt_cb_t    evt_param;

    CHECK_BTHL_INIT();
    BTIF_TRACE_EVENT("%s app_id=%d", __FUNCTION__, app_id);
    btif_hl_display_calling_process_name();

 if (btif_hl_find_app_idx(((UINT8)app_id), &app_idx))
 {
        evt_param.unreg.app_idx = app_idx;
        BTIF_HL_GET_APP_CB_PTR(app_idx);
        reg_counter --;
        len = sizeof(btif_hl_unreg_t);
        status = btif_transfer_context (btif_hl_proc_cb_evt, BTIF_HL_UNREG_APP,
 (char*) &evt_param, len, NULL);
        ASSERTC(status == BT_STATUS_SUCCESS, "context transfer failed", status);
 }
 else
 {
        status  = BT_STATUS_FAIL;
 }

    BTIF_TRACE_DEBUG("de-reg return status=%d", status);
 return status;
}
