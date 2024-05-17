BOOLEAN btif_hl_find_app_idx_using_mdepId(UINT8 mdep_id, UINT8 *p_app_idx){
    BOOLEAN found=FALSE;
    UINT8 i;

 *p_app_idx = 0;
 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
        BTIF_TRACE_DEBUG("btif_hl_find_app_idx_using_mdepId: MDEP-ID = %d",
                btif_hl_cb.acb[i].sup_feature.mdep[0].mdep_id);
 if (btif_hl_cb.acb[i].in_use &&
 (btif_hl_cb.acb[i].sup_feature.mdep[0].mdep_id == mdep_id))
 {
            found = TRUE;
 *p_app_idx = i;
 break;
 }
 }
    BTIF_TRACE_DEBUG("%s found=%d app_idx=%d", __FUNCTION__, found, i );

 return found;
}
