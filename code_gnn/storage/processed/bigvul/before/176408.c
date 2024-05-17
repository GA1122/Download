static void btif_report_audio_state(btav_audio_state_t state,
 RawAddress* bd_addr) {
 if (bt_av_sink_callbacks != NULL) {
    HAL_CBACK(bt_av_sink_callbacks, audio_state_cb, state, bd_addr);
 } else if (bt_av_src_callbacks != NULL) {
    HAL_CBACK(bt_av_src_callbacks, audio_state_cb, state, bd_addr);
 }
}
