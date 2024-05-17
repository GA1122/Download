static void btif_hl_disable(void){
    BTIF_TRACE_DEBUG("%s", __FUNCTION__);

 if ((p_btif_hl_cb->state != BTIF_HL_STATE_DISABLING) &&
 (p_btif_hl_cb->state != BTIF_HL_STATE_DISABLED))
 {
        btif_hl_set_state(BTIF_HL_STATE_DISABLING);
        BTA_HlDisable();
 }
}
