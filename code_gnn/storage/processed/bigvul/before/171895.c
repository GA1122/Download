static void btif_hl_proc_cch_close_ind(tBTA_HL *p_data)

{
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

    btif_hl_clean_mcb_using_handle(p_data->cch_close_ind.mcl_handle);
}
