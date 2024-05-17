static BOOLEAN btif_hl_get_bta_channel_type(bthl_channel_type_t channel_type, tBTA_HL_DCH_CFG *p){
    BOOLEAN status = TRUE;
 switch (channel_type)
 {
 case BTHL_CHANNEL_TYPE_RELIABLE:
 *p = BTA_HL_DCH_CFG_RELIABLE;
 break;
 case BTHL_CHANNEL_TYPE_STREAMING:
 *p = BTA_HL_DCH_CFG_STREAMING;
 break;
 case BTHL_CHANNEL_TYPE_ANY:
 *p = BTA_HL_DCH_CFG_NO_PREF;
 break;
 default:
            status = FALSE;
 break;
 }
    BTIF_TRACE_DEBUG("%s status = %d BTA DCH CFG=%d (1-rel 2-strm",
                      __FUNCTION__, status, *p);
 return status;
}
