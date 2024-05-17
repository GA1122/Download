static void btif_hl_set_state(btif_hl_state_t state){
    BTIF_TRACE_DEBUG("btif_hl_set_state:  %d ---> %d ", p_btif_hl_cb->state, state);
    p_btif_hl_cb->state = state;
}
