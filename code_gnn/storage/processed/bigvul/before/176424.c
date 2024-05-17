static void update_audio_focus_state(int state) {
  BTIF_TRACE_DEBUG("%s: state=%d", __func__, state);
  btif_a2dp_sink_set_focus_state_req((btif_a2dp_sink_focus_state_t)state);
}
