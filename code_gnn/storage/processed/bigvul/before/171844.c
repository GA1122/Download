void  btif_hl_dch_abort(UINT8 app_idx, UINT8 mcl_idx){
 btif_hl_mcl_cb_t *p_mcb;

    BTIF_TRACE_DEBUG("%s app_idx=%d mcl_idx=%d",__FUNCTION__, app_idx, mcl_idx );
    p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
 if (p_mcb->is_connected)
 {
        BTA_HlDchAbort(p_mcb->mcl_handle);
 }
 else
 {
        p_mcb->pcb.abort_pending = TRUE;
 }

}
