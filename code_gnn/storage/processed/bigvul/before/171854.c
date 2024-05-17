static BOOLEAN btif_hl_find_avail_app_idx(UINT8 *p_idx){
    BOOLEAN found = FALSE;
    UINT8 i;

 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
 if (!btif_hl_cb.acb[i].in_use)
 {
            found = TRUE;
 *p_idx = i;
 break;
 }
 }

    BTIF_TRACE_DEBUG("%s found=%d app_idx=%d", __FUNCTION__, found, i);
 return found;
}
