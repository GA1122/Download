static void btif_hl_init(void){
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);
    memset(p_btif_hl_cb, 0, sizeof(btif_hl_cb_t));
    btif_hl_init_next_app_id();
    btif_hl_init_next_channel_id();
}
