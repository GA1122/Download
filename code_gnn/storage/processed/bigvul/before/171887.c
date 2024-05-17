BOOLEAN btif_hl_is_reconnect_possible(UINT8 app_idx, UINT8 mcl_idx, int mdep_cfg_idx,
                                      tBTA_HL_DCH_OPEN_PARAM *p_dch_open_api, tBTA_HL_MDL_ID *p_mdl_id)
{
 btif_hl_app_cb_t *p_acb = BTIF_HL_GET_APP_CB_PTR(app_idx);
 btif_hl_mcl_cb_t *p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    tBTA_HL_DCH_CFG      local_cfg = p_dch_open_api->local_cfg;
    tBTA_HL_DCH_MODE     dch_mode = BTA_HL_DCH_MODE_RELIABLE;
    BOOLEAN              use_mdl_dch_mode=FALSE;
 btif_hl_mdl_cfg_t *p_mdl;
 btif_hl_mdl_cfg_t *p_mdl1;
    UINT8                i, j;
    BOOLEAN              is_reconnect_ok=FALSE;
    BOOLEAN              stream_mode_avail=FALSE;
    UINT16               data_type = p_acb->sup_feature.mdep[mdep_cfg_idx].mdep_cfg.data_cfg[0].data_type;
    tBTA_HL_MDEP_ID      peer_mdep_id = p_dch_open_api->peer_mdep_id;
    UINT8                mdl_idx;


    BTIF_TRACE_DEBUG("%s app_idx=%d mcl_idx=%d mdep_cfg_idx=%d",
                      __FUNCTION__, app_idx, mcl_idx, mdep_cfg_idx  );
 switch (local_cfg)
 {
 case BTA_HL_DCH_CFG_NO_PREF:
 if (!btif_hl_is_the_first_reliable_existed(app_idx, mcl_idx))
 {
                dch_mode = BTA_HL_DCH_MODE_RELIABLE;
 }
 else
 {
                use_mdl_dch_mode = TRUE;
 }
 break;
 case BTA_HL_DCH_CFG_RELIABLE:
            dch_mode = BTA_HL_DCH_MODE_RELIABLE;
 break;
 case BTA_HL_DCH_CFG_STREAMING:
            dch_mode = BTA_HL_DCH_MODE_STREAMING;
 break;
 default:
            BTIF_TRACE_ERROR("Invalid local_cfg=%d",local_cfg );
 return is_reconnect_ok;
 break;

 }

    BTIF_TRACE_DEBUG("local_cfg=%d use_mdl_dch_mode=%d dch_mode=%d ",
                      local_cfg, use_mdl_dch_mode, dch_mode  );

 for (i=0, p_mdl=&p_acb->mdl_cfg[0] ; i< BTA_HL_NUM_MDL_CFGS; i++, p_mdl++ )
 {
 if (p_mdl->base.active &&
            p_mdl->extra.data_type ==data_type &&
 (p_mdl->extra.peer_mdep_id != BTA_HL_INVALID_MDEP_ID && p_mdl->extra.peer_mdep_id == peer_mdep_id) &&
            memcpy(p_mdl->base.peer_bd_addr, p_mcb->bd_addr,sizeof(BD_ADDR) ) &&
            p_mdl->base.mdl_id &&
 !btif_hl_find_mdl_idx(app_idx, mcl_idx,p_mdl->base.mdl_id, &mdl_idx))
 {
            BTIF_TRACE_DEBUG("i=%d Matched active=%d   mdl_id =%d, mdl_dch_mode=%d",
                              i, p_mdl->base.active, p_mdl->base.mdl_id,p_mdl->base.dch_mode);
 if (!use_mdl_dch_mode)
 {
 if (p_mdl->base.dch_mode == dch_mode)
 {
                    is_reconnect_ok = TRUE;
 *p_mdl_id = p_mdl->base.mdl_id;
                    BTIF_TRACE_DEBUG("reconnect is possible dch_mode=%d mdl_id=%d", dch_mode, p_mdl->base.mdl_id );
 break;
 }
 }
 else
 {
                is_reconnect_ok = TRUE;
 for (j=i, p_mdl1=&p_acb->mdl_cfg[i]; j< BTA_HL_NUM_MDL_CFGS; j++, p_mdl1++)
 {
 if (p_mdl1->base.active &&
                        p_mdl1->extra.data_type == data_type &&
 (p_mdl1->extra.peer_mdep_id != BTA_HL_INVALID_MDEP_ID && p_mdl1->extra.peer_mdep_id == peer_mdep_id) &&
                        memcpy(p_mdl1->base.peer_bd_addr, p_mcb->bd_addr,sizeof(BD_ADDR)) &&
                        p_mdl1->base.dch_mode == BTA_HL_DCH_MODE_STREAMING)
 {
                        stream_mode_avail = TRUE;
                        BTIF_TRACE_DEBUG("found streaming mode mdl index=%d", j);
 break;
 }
 }

 if (stream_mode_avail)
 {
                    dch_mode = BTA_HL_DCH_MODE_STREAMING;
 *p_mdl_id = p_mdl1->base.mdl_id;
                    BTIF_TRACE_DEBUG("reconnect is ok index=%d dch_mode=streaming  mdl_id=%d", j, *p_mdl_id);
 break;
 }
 else
 {
                    dch_mode= p_mdl->base.dch_mode;
 *p_mdl_id = p_mdl->base.mdl_id;
                    BTIF_TRACE_DEBUG("reconnect is ok index=%d  dch_mode=%d mdl_id=%d", i,  p_mdl->base.dch_mode, *p_mdl_id);
 break;

 }
 }

 }

 }

    BTIF_TRACE_DEBUG("is_reconnect_ok  dch_mode=%d mdl_id=%d",is_reconnect_ok, dch_mode, *p_mdl_id);
 return is_reconnect_ok;
}
