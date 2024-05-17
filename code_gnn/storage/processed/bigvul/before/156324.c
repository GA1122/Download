String StateToString(MediaRecorder::State state) {
  switch (state) {
    case MediaRecorder::State::kInactive:
      return "inactive";
    case MediaRecorder::State::kRecording:
      return "recording";
    case MediaRecorder::State::kPaused:
      return "paused";
  }

  NOTREACHED();
  return String();
}
