static UINT8 btif_hl_get_app_id(int channel_id){
    UINT8 app_id =(UINT8) (channel_id >> 16);
    BTIF_TRACE_DEBUG("%s channel_id=0x%08x, app_id=0x%02x ", __FUNCTION__,channel_id, app_id);
 return app_id;
}
