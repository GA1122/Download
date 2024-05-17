static btif_hl_state_t btif_hl_get_state(void){
    BTIF_TRACE_DEBUG("btif_hl_get_state:  %d   ", p_btif_hl_cb->state);
 return p_btif_hl_cb->state;
}
