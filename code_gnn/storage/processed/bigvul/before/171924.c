void btif_hl_set_socket_state(btif_hl_soc_cb_t *p_scb, btif_hl_soc_state_t new_state){
    BTIF_TRACE_DEBUG("btif_hl_set_socket_state %d---->%d", p_scb->state, new_state);
    p_scb->state = new_state;
}
