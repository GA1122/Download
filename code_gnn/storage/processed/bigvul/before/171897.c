static void btif_hl_proc_cch_open_ind(tBTA_HL *p_data)

{
 btif_hl_mcl_cb_t *p_mcb;
    UINT8                   mcl_idx;
 int                     i;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
 for(i=0; i<BTA_HL_NUM_APPS; i++)
 {
 if (btif_hl_cb.acb[i].in_use)
 {
 if (!btif_hl_find_mcl_idx(i, p_data->cch_open_ind.bd_addr, &mcl_idx))
 {
 if (btif_hl_find_avail_mcl_idx(i, &mcl_idx))
 {
                    p_mcb = BTIF_HL_GET_MCL_CB_PTR(i, mcl_idx);
                    memset(p_mcb, 0, sizeof(btif_hl_mcl_cb_t));
                    p_mcb->in_use = TRUE;
                    p_mcb->is_connected = TRUE;
                    p_mcb->mcl_handle = p_data->cch_open_ind.mcl_handle;
                    bdcpy(p_mcb->bd_addr, p_data->cch_open_ind.bd_addr);
                    btif_hl_start_cch_timer(i, mcl_idx);
 }
 }
 else
 {
                BTIF_TRACE_ERROR("The MCL already exist for cch_open_ind");
 }
 }
 }
}
