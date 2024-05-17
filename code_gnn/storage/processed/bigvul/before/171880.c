btif_hl_soc_state_t btif_hl_get_socket_state(btif_hl_soc_cb_t *p_scb){
    BTIF_TRACE_DEBUG("btif_hl_get_socket_state state=%d", p_scb->state);
 return p_scb->state;
}
