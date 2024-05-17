const char* AudioPlayingStateToString(bool is_audio_playing) {
  if (is_audio_playing) {
    return "playing";
  } else {
    return "muted";
  }
}
