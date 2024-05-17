BOOLEAN btif_hl_find_avail_mcl_idx(UINT8 app_idx, UINT8 *p_mcl_idx){
    BOOLEAN found=FALSE;
    UINT8 i;

 for (i=0; i < BTA_HL_NUM_MCLS ; i ++)
 {
 if (!btif_hl_cb.acb[app_idx].mcb[i].in_use)
 {
            found = TRUE;
 *p_mcl_idx = i;
 break;
 }
 }
    BTIF_TRACE_DEBUG("%s found=%d mcl_idx=%d", __FUNCTION__, found, i);
 return found;
}
