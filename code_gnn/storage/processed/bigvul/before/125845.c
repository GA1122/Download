void AddHistogramFramesPerBuffer(int param) {
  AudioFramesPerBuffer afpb = AsAudioFramesPerBuffer(param);
  if (afpb != kUnexpectedAudioBufferSize) {
    UMA_HISTOGRAM_ENUMERATION("WebRTC.AudioOutputFramesPerBuffer",
                              afpb, kUnexpectedAudioBufferSize);
  } else {
    UMA_HISTOGRAM_COUNTS("WebRTC.AudioOutputFramesPerBufferUnexpected", param);
  }
}
