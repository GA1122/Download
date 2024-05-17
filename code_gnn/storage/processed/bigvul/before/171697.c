void bta_hl_co_save_mdl(UINT8 mdep_id, UINT8 item_idx, tBTA_HL_MDL_CFG *p_mdl_cfg )
{

    BTIF_TRACE_DEBUG("%s mdep_id =%d, item_idx=%d active=%d mdl_id=%d time=%d",
                      __FUNCTION__, mdep_id, item_idx,
                      p_mdl_cfg->active,
                      p_mdl_cfg->mdl_id,
                      p_mdl_cfg->time);

    btif_hl_save_mdl_cfg(mdep_id, item_idx, p_mdl_cfg);

}
