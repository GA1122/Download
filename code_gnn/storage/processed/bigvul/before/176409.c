static void btif_report_connection_state(btav_connection_state_t state,
 RawAddress* bd_addr) {
 if (bt_av_sink_callbacks != NULL) {
    HAL_CBACK(bt_av_sink_callbacks, connection_state_cb, state, bd_addr);
 } else if (bt_av_src_callbacks != NULL) {
    HAL_CBACK(bt_av_src_callbacks, connection_state_cb, state, bd_addr);
 }
}
