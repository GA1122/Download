void btif_hl_select_wakeup_callback( fd_set *p_org_set , int wakeup_signal){
    BTIF_TRACE_DEBUG("entering %s wakeup_signal=0x%04x",__FUNCTION__, wakeup_signal);

 if (wakeup_signal == btif_hl_signal_select_wakeup )
 {
        btif_hl_add_socket_to_set(p_org_set);
 }
 else if (wakeup_signal == btif_hl_signal_select_close_connected)
 {
        btif_hl_close_socket(p_org_set);
 }
    BTIF_TRACE_DEBUG("leaving %s",__FUNCTION__);
}
