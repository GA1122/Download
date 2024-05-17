BOOLEAN btif_hl_find_mcl_idx(UINT8 app_idx, BD_ADDR p_bd_addr, UINT8 *p_mcl_idx){
    BOOLEAN found=FALSE;
    UINT8 i;
 btif_hl_mcl_cb_t *p_mcb;

 *p_mcl_idx = 0;
 for (i=0; i < BTA_HL_NUM_MCLS ; i ++)
 {
        p_mcb = BTIF_HL_GET_MCL_CB_PTR(app_idx, i);
 if (p_mcb->in_use &&
 (!memcmp (p_mcb->bd_addr, p_bd_addr, BD_ADDR_LEN)))
 {
            found = TRUE;
 *p_mcl_idx = i;
 break;
 }
 }


    BTIF_TRACE_DEBUG("%s found=%d idx=%d",__FUNCTION__, found, i);
 return found;
}
