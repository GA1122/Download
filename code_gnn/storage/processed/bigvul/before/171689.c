void bta_hl_co_delete_mdl(UINT8 mdep_id, UINT8 item_idx)
{


    BTIF_TRACE_DEBUG("%s mdep_id=%d, item_idx=%d", __FUNCTION__, mdep_id, item_idx);

    btif_hl_delete_mdl_cfg(mdep_id, item_idx);


}
