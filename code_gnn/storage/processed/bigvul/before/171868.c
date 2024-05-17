static BOOLEAN btif_hl_find_sdp_idx_using_mdep_filter(UINT8 app_idx, UINT8 mcl_idx, UINT8 *p_sdp_idx)
{
 btif_hl_app_cb_t *p_acb  =BTIF_HL_GET_APP_CB_PTR(app_idx);
 btif_hl_mcl_cb_t *p_mcb  =BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    UINT8                   i, j, num_recs,num_elems, num_mdeps, mdep_idx;
    tBTA_HL_MDEP_ROLE       peer_mdep_role;
    UINT16                  data_type;
    tBTA_HL_SDP_MDEP_CFG    *p_mdep;
    BOOLEAN                 found = FALSE;
    BOOLEAN                 elem_found;

    BTIF_TRACE_DEBUG("btif_hl_find_sdp_idx_using_mdep_filter");
    num_recs = p_mcb->sdp.num_recs;
    num_elems = p_acb->filter.num_elems;
 if (!num_elems)
 {
        BTIF_TRACE_DEBUG("btif_hl_find_sdp_idx_using_mdep_filter num_elem=0");
 *p_sdp_idx = 0;
        found = TRUE;
 return found;
 }

 for (i=0; i<num_recs; i++)
 {
        num_mdeps = p_mcb->sdp.sdp_rec[i].num_mdeps;
 for (j=0; j<num_elems; j++ )
 {
            data_type = p_acb->filter.elem[j].data_type;
            peer_mdep_role = p_acb->filter.elem[j].peer_mdep_role;
            elem_found = FALSE;
            mdep_idx=0;
 while (!elem_found && mdep_idx < num_mdeps )
 {
                p_mdep = &(p_mcb->sdp.sdp_rec[i].mdep_cfg[mdep_idx]);
 if ( (p_mdep->data_type == data_type) &&
 (p_mdep->mdep_role == peer_mdep_role) )
 {
                    elem_found = TRUE;
 }
 else
 {
                    mdep_idx++;
 }
 }

 if (!elem_found)
 {
                found = FALSE;
 break;
 }
 else
 {
                found = TRUE;
 }
 }

 if (found)
 {
            BTIF_TRACE_DEBUG("btif_hl_find_sdp_idx_using_mdep_filter found idx=%d",i);
 *p_sdp_idx = i;
 break;
 }
 }

    BTIF_TRACE_DEBUG("%s found=%d sdp_idx=%d",__FUNCTION__ , found, *p_sdp_idx);

    btif_hl_reset_mdep_filter(app_idx);

 return found;
}
