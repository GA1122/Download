BOOLEAN btif_hl_find_app_idx_using_handle(tBTA_HL_APP_HANDLE app_handle,
                                          UINT8 *p_app_idx){
    BOOLEAN found=FALSE;
    UINT8 i;

 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
 if (btif_hl_cb.acb[i].in_use &&
 (btif_hl_cb.acb[i].app_handle == app_handle))
 {
            found = TRUE;
 *p_app_idx = i;
 break;
 }
 }

    BTIF_TRACE_EVENT("%s status=%d handle=%d app_idx=%d ",
                      __FUNCTION__, found, app_handle , i);

 return found;
}
