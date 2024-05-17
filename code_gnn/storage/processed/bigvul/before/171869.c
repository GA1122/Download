static void btif_hl_free_app_idx(UINT8 app_idx){

 if ((app_idx < BTA_HL_NUM_APPS) && btif_hl_cb.acb[app_idx].in_use )
 {
        btif_hl_cb.acb[app_idx].in_use = FALSE;
        memset (&btif_hl_cb.acb[app_idx], 0, sizeof(btif_hl_app_cb_t));
 }
}
