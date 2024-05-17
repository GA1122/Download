static void avrc_ctrl_cback(uint8_t handle, uint8_t event, uint16_t result,
 const RawAddress* peer_addr) {
 uint8_t avrc_event;

 if (event <= AVRC_MAX_RCV_CTRL_EVT && avrc_cb.ccb[handle].ctrl_cback) {
    avrc_event = avrc_ctrl_event_map[event];
 if (event == AVCT_CONNECT_CFM_EVT) {
 if (result != 0)  
        avrc_event = AVRC_CLOSE_IND_EVT;
 }
    avrc_cb.ccb[handle].ctrl_cback.Run(handle, avrc_event, result, peer_addr);
 }

 if ((event == AVCT_DISCONNECT_CFM_EVT) ||
 (event == AVCT_DISCONNECT_IND_EVT)) {
    avrc_flush_cmd_q(handle);
    alarm_free(avrc_cb.ccb_int[handle].tle);
    avrc_cb.ccb_int[handle].tle = NULL;
 }
}
