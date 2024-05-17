static BOOLEAN btif_hl_find_mdl_idx(UINT8 app_idx, UINT8 mcl_idx, UINT16 mdl_id,
                                    UINT8 *p_mdl_idx)
{
 btif_hl_mcl_cb_t *p_mcb  = BTIF_HL_GET_MCL_CB_PTR(app_idx, mcl_idx);
    BOOLEAN found=FALSE;
    UINT8 i;

 for (i=0; i < BTA_HL_NUM_MDLS_PER_MCL ; i ++)
 {
 if (p_mcb->mdl[i].in_use  &&
 (mdl_id !=0) &&
 (p_mcb->mdl[i].mdl_id== mdl_id))
 {
            found = TRUE;
 *p_mdl_idx = i;
 break;
 }
 }

    BTIF_TRACE_DEBUG("%s found=%d mdl_id=%d mdl_idx=%d ",
                      __FUNCTION__,found, mdl_id, i);

 return found;
}
