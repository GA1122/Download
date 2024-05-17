static void btif_hl_proc_dch_cong_ind(tBTA_HL *p_data)

{
 btif_hl_mdl_cb_t *p_dcb;
    UINT8                   app_idx, mcl_idx, mdl_idx;

    BTIF_TRACE_DEBUG("btif_hl_proc_dch_cong_ind");


 if (btif_hl_find_mdl_idx_using_handle(p_data->dch_cong_ind.mdl_handle, &app_idx, &mcl_idx, &mdl_idx))
 {
        p_dcb =BTIF_HL_GET_MDL_CB_PTR(app_idx, mcl_idx, mdl_idx);
        p_dcb->cong = p_data->dch_cong_ind.cong;
 }
}
