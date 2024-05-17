static void btif_hl_proc_create_ind(tBTA_HL *p_data){
 btif_hl_app_cb_t *p_acb;
 btif_hl_mcl_cb_t *p_mcb;
    tBTA_HL_MDEP            *p_mdep;
    UINT8                   orig_app_idx, mcl_idx, mdep_cfg_idx;
    BOOLEAN                 first_reliable_exist;
    BOOLEAN                 success = TRUE;
    tBTA_HL_DCH_CFG         rsp_cfg = BTA_HL_DCH_CFG_UNKNOWN;
    tBTA_HL_DCH_CREATE_RSP  rsp_code = BTA_HL_DCH_CREATE_RSP_CFG_REJ;
    tBTA_HL_DCH_CREATE_RSP_PARAM create_rsp_param;

    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

    btif_hl_find_app_idx_using_mdepId(p_data->dch_create_ind.local_mdep_id,&orig_app_idx);
 if (btif_hl_find_mcl_idx(orig_app_idx, p_data->dch_create_ind.bd_addr, &mcl_idx))
 {
        p_acb =BTIF_HL_GET_APP_CB_PTR(orig_app_idx);
        p_mcb =BTIF_HL_GET_MCL_CB_PTR(orig_app_idx, mcl_idx);

 if (btif_hl_find_mdep_cfg_idx(orig_app_idx, p_data->dch_create_ind.local_mdep_id, &mdep_cfg_idx))
 {
            p_mdep = &(p_acb->sup_feature.mdep[mdep_cfg_idx]);
            first_reliable_exist = btif_hl_is_the_first_reliable_existed(orig_app_idx, mcl_idx);
 switch (p_mdep->mdep_cfg.mdep_role)
 {
 case BTA_HL_MDEP_ROLE_SOURCE:
 if (p_data->dch_create_ind.cfg == BTA_HL_DCH_CFG_NO_PREF)
 {
 if (first_reliable_exist)
 {
                            rsp_cfg = p_acb->channel_type[mdep_cfg_idx];
 }
 else
 {
                            rsp_cfg = BTA_HL_DCH_CFG_RELIABLE;
 }
                        rsp_code = BTA_HL_DCH_CREATE_RSP_SUCCESS;
 }

 break;
 case BTA_HL_MDEP_ROLE_SINK:

                    BTIF_TRACE_DEBUG("btif_hl_proc_create_ind:BTA_HL_MDEP_ROLE_SINK");
 if ((p_data->dch_create_ind.cfg  == BTA_HL_DCH_CFG_RELIABLE) ||
 (first_reliable_exist && (p_data->dch_create_ind.cfg  == BTA_HL_DCH_CFG_STREAMING)))
 {
                        rsp_code = BTA_HL_DCH_CREATE_RSP_SUCCESS;
                        rsp_cfg = p_data->dch_create_ind.cfg;
                        BTIF_TRACE_DEBUG("btif_hl_proc_create_ind:BTA_HL_MDEP_ROLE_SINK cfg = %d",rsp_cfg);
 }
 break;
 default:
 break;
 }
 }
 }
 else
 {
        success = FALSE;
 }

 if (success)
 {
        BTIF_TRACE_DEBUG("create response rsp_code=%d rsp_cfg=%d", rsp_code, rsp_cfg );
        create_rsp_param.local_mdep_id = p_data->dch_create_ind.local_mdep_id;
        create_rsp_param.mdl_id = p_data->dch_create_ind.mdl_id;
        create_rsp_param.rsp_code = rsp_code;
        create_rsp_param.cfg_rsp = rsp_cfg;
        BTA_HlDchCreateRsp(p_mcb->mcl_handle, &create_rsp_param);
 }
}
