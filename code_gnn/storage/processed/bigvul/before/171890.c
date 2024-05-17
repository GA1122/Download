UINT8 btif_hl_num_dchs_in_use(UINT8 mcl_handle){

 btif_hl_app_cb_t * p_acb;
 btif_hl_mcl_cb_t *p_mcb;
    UINT8               i,j,x;
    UINT8               cnt=0;

 for (i=0; i<BTA_HL_NUM_APPS; i++)
 {
        BTIF_TRACE_DEBUG("btif_hl_num_dchs:i = %d",i);
        p_acb =BTIF_HL_GET_APP_CB_PTR(i);
 if (p_acb && p_acb->in_use)
 {
 for (j=0; j < BTA_HL_NUM_MCLS ; j++)
 {
 if(p_acb->mcb[j].in_use)
                    BTIF_TRACE_DEBUG("btif_hl_num_dchs:mcb in use j=%d, mcl_handle=%d,mcb handle=%d",
                                        j,mcl_handle, p_acb->mcb[j].mcl_handle);
 if (p_acb->mcb[j].in_use &&
 (p_acb->mcb[j].mcl_handle == mcl_handle))
 {
                    p_mcb = &p_acb->mcb[j];
                    BTIF_TRACE_DEBUG("btif_hl_num_dchs: mcl handle found j =%d",j);
 for (x=0; x < BTA_HL_NUM_MDLS_PER_MCL ; x ++)
 {
 if (p_mcb->mdl[x].in_use)
 {
                            BTIF_TRACE_DEBUG("btif_hl_num_dchs_in_use:found x =%d",x);
                            cnt++;
 }
 }
 }
 }
 }
 }

    BTIF_TRACE_DEBUG("%s dch in use count=%d", __FUNCTION__, cnt);
 return cnt;
}
