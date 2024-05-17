static BOOLEAN btif_hl_proc_cch_open_cfm(tBTA_HL *p_data)

{
 btif_hl_mcl_cb_t *p_mcb;
    UINT8                    app_idx, mcl_idx;
    BOOLEAN                  status = FALSE;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

 if (btif_hl_find_app_idx_using_app_id(p_data->cch_open_cfm.app_id, &app_idx))
 {
        BTIF_TRACE_DEBUG("app_idx=%d", app_idx);
 if (btif_hl_find_mcl_idx(app_idx, p_data->cch_open_cfm.bd_addr, &mcl_idx))
 {
            BTIF_HL_GET_APP_CB_PTR(app_idx);

            p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
            BTIF_TRACE_DEBUG("mcl_idx=%d, mcl_handle=%d", mcl_idx,p_data->cch_open_cfm.mcl_handle);
            p_mcb->mcl_handle = p_data->cch_open_cfm.mcl_handle;
            p_mcb->is_connected = TRUE;
            status = btif_hl_proc_pending_op(app_idx, mcl_idx);
 if (status)
                btif_hl_start_cch_timer(app_idx, mcl_idx);
 }
 }

 return status;
}
