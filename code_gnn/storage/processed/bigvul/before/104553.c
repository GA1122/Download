 bool ExtensionTtsPlatformImplWin::StopSpeaking() {
  if (speech_synthesizer_ && !paused_) {
    speech_synthesizer_->Pause();
    paused_ = true;
  }
  return true;
}
