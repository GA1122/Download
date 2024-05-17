BOOLEAN bta_hl_co_get_num_of_mdep(UINT8 app_id, UINT8 *p_num_of_mdep)
{
    UINT8 app_idx;
    BOOLEAN success = FALSE;

 if (btif_hl_find_app_idx(app_id, &app_idx))
 {
 *p_num_of_mdep = p_btif_hl_cb->acb[app_idx].sup_feature.num_of_mdeps;
        success = TRUE;
 }


    BTIF_TRACE_DEBUG("%s success=%d num_mdeps=%d",
                      __FUNCTION__, success, *p_num_of_mdep );
 return success;
}
