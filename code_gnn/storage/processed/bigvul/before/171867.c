static BOOLEAN btif_hl_find_peer_mdep_id(UINT8 app_id, BD_ADDR bd_addr,
                                         tBTA_HL_MDEP_ROLE local_mdep_role,
                                         UINT16 data_type,
                                         tBTA_HL_MDEP_ID *p_peer_mdep_id){
    UINT8               app_idx, mcl_idx;
 btif_hl_mcl_cb_t *p_mcb;
    tBTA_HL_SDP_REC     *p_rec;
    UINT8               i, num_mdeps;
    BOOLEAN             found = FALSE;
    tBTA_HL_MDEP_ROLE   peer_mdep_role;


    BTIF_TRACE_DEBUG("%s app_id=%d local_mdep_role=%d, data_type=%d",
                      __FUNCTION__, app_id, local_mdep_role, data_type);

    BTIF_TRACE_DEBUG("DB [%02x:%02x:%02x:%02x:%02x:%02x]",
                      bd_addr[0],  bd_addr[1],
                      bd_addr[2],  bd_addr[3],
                      bd_addr[4],  bd_addr[5]);


    BTIF_TRACE_DEBUG("local_mdep_role=%d", local_mdep_role);
    BTIF_TRACE_DEBUG("data_type=%d", data_type);

 if (local_mdep_role == BTA_HL_MDEP_ROLE_SINK)
        peer_mdep_role = BTA_HL_MDEP_ROLE_SOURCE;
 else
        peer_mdep_role = BTA_HL_MDEP_ROLE_SINK;

 if (btif_hl_find_app_idx(app_id, &app_idx) )
 {
        BTIF_HL_GET_APP_CB_PTR(app_idx);
 if (btif_hl_find_mcl_idx(app_idx, bd_addr, &mcl_idx))
 {
            p_mcb  =BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);

            BTIF_TRACE_DEBUG("app_idx=%d mcl_idx=%d",app_idx, mcl_idx);
            BTIF_TRACE_DEBUG("valid_spd_idx=%d sdp_idx=%d",p_mcb->valid_sdp_idx, p_mcb->sdp_idx);
 if (p_mcb->valid_sdp_idx)
 {
                p_rec = &p_mcb->sdp.sdp_rec[p_mcb->sdp_idx];
                num_mdeps = p_rec->num_mdeps;
                BTIF_TRACE_DEBUG("num_mdeps=%d", num_mdeps);

 for (i=0; i< num_mdeps; i++)
 {
                    BTIF_TRACE_DEBUG("p_rec->mdep_cfg[%d].mdep_role=%d",i, p_rec->mdep_cfg[i].mdep_role);
                    BTIF_TRACE_DEBUG("p_rec->mdep_cfg[%d].data_type =%d",i, p_rec->mdep_cfg[i].data_type );
 if ((p_rec->mdep_cfg[i].mdep_role == peer_mdep_role) &&
 (p_rec->mdep_cfg[i].data_type == data_type))
 {
                        found = TRUE;
 *p_peer_mdep_id = p_rec->mdep_cfg[i].mdep_id;
 break;
 }
 }
 }
 }
 }

    BTIF_TRACE_DEBUG("found =%d  *p_peer_mdep_id=%d", found, *p_peer_mdep_id);

 return found;
}
