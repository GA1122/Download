static BOOLEAN btif_hl_is_no_active_app(void){
    BOOLEAN no_active_app = TRUE;
    UINT8 i;

 for (i=0; i < BTA_HL_NUM_APPS ; i ++)
 {
 if (btif_hl_cb.acb[i].in_use)
 {
            no_active_app = FALSE;
 break;
 }
 }

    BTIF_TRACE_DEBUG("%s no_active_app=%d  ", __FUNCTION__, no_active_app );
 return no_active_app;
}
