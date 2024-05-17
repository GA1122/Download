static int btif_hl_get_next_channel_id(UINT8 app_id){
    UINT16 next_channel_id = btif_hl_cb.next_channel_id;
 int channel_id;
    btif_hl_cb.next_channel_id++;
    channel_id = (app_id << 16) + next_channel_id;
    BTIF_TRACE_DEBUG("%s channel_id=0x%08x, app_id=0x%02x next_channel_id=0x%04x", __FUNCTION__,
                      channel_id, app_id,  next_channel_id);
 return channel_id;
}
