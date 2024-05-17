bool btif_av_is_connected(void) {
 btif_sm_state_t state = btif_sm_get_state(btif_av_cb.sm_handle);
 return ((state == BTIF_AV_STATE_OPENED) || (state == BTIF_AV_STATE_STARTED));
}
