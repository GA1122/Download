BOOLEAN btif_hl_is_the_first_reliable_existed(UINT8 app_idx, UINT8 mcl_idx )
{
 btif_hl_mcl_cb_t *p_mcb  =BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    BOOLEAN is_existed =FALSE;
    UINT8 i ;

 for (i=0; i< BTA_HL_NUM_MDLS_PER_MCL; i++)
 {
 if (p_mcb->mdl[i].in_use && p_mcb->mdl[i].is_the_first_reliable)
 {
            is_existed = TRUE;
 break;
 }
 }

    BTIF_TRACE_DEBUG("bta_hl_is_the_first_reliable_existed is_existed=%d  ",is_existed );
 return is_existed;
}
