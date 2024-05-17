static void update_audio_track_gain(float gain) {
  BTIF_TRACE_DEBUG("%s: gain=%f", __func__, gain);
  btif_a2dp_sink_set_audio_track_gain(gain);
}
