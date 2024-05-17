BOOLEAN bta_hl_co_get_mdep_config(UINT8  app_id,
                                  UINT8 mdep_idx,
                                  UINT8 mdep_counter,
                                  tBTA_HL_MDEP_ID mdep_id,
                                  tBTA_HL_MDEP_CFG *p_mdep_cfg)
{
    UINT8       idx  ;
    UINT8       app_idx;
    BOOLEAN     success = FALSE;

    BTIF_TRACE_DEBUG("%s app_id=%d mdep_idx=%d mdep_id=%d mdep_counter=%d",
                      __FUNCTION__, app_id,mdep_idx,mdep_id,mdep_counter);

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
        idx = mdep_idx -mdep_counter-1;
        p_btif_hl_cb->acb[app_idx].sup_feature.mdep[idx].mdep_id = mdep_id;
        memcpy(p_mdep_cfg,
 &p_btif_hl_cb->acb[app_idx].sup_feature.mdep[idx].mdep_cfg,
 sizeof(tBTA_HL_MDEP_CFG));

        success = TRUE;
 }

    BTIF_TRACE_DEBUG("%s success=%d mdep_idx=%d mdep_id=%d",
                      __FUNCTION__, success, mdep_idx, mdep_id );

 return success;
}
