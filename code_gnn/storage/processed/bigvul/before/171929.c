static void btif_hl_stop_timer_using_handle( tBTA_HL_MCL_HANDLE mcl_handle){
 btif_hl_app_cb_t *p_acb;
    UINT8 i,j;

 for (i=0; i<BTA_HL_NUM_APPS; i++)
 {
        p_acb =BTIF_HL_GET_APP_CB_PTR(i);
 for (j=0; j < BTA_HL_NUM_MCLS ; j++)
 {
 if (p_acb->mcb[j].in_use &&
 (p_acb->mcb[j].mcl_handle == mcl_handle))
 {
                btif_hl_stop_cch_timer(i, j);
 }
 }
 }
}
