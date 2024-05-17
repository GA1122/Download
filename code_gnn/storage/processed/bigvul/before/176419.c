const char* dump_av_sm_state_name(btif_av_state_t state) {
 switch (state) {
    CASE_RETURN_STR(BTIF_AV_STATE_IDLE)
    CASE_RETURN_STR(BTIF_AV_STATE_OPENING)
    CASE_RETURN_STR(BTIF_AV_STATE_OPENED)
    CASE_RETURN_STR(BTIF_AV_STATE_STARTED)
    CASE_RETURN_STR(BTIF_AV_STATE_CLOSING)
 default:
 return "UNKNOWN_STATE";
 }
}
