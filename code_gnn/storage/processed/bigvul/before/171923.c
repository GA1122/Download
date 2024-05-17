void btif_hl_set_chan_cb_state(UINT8 app_idx, UINT8 mcl_idx, btif_hl_chan_cb_state_t state){
 btif_hl_pending_chan_cb_t *p_pcb = BTIF_HL_GET_PCB_PTR(app_idx, mcl_idx);
 btif_hl_chan_cb_state_t cur_state = p_pcb->cb_state;

 if (cur_state != state)
 {
        p_pcb->cb_state = state;
        BTIF_TRACE_DEBUG("%s state %d--->%d",__FUNCTION__, cur_state, state);
 }


}
