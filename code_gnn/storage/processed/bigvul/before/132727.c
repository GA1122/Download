void ChromotingInstance::HandlePauseAudio(const base::DictionaryValue& data) {
  bool pause = false;
  if (!data.GetBoolean("pause", &pause)) {
    LOG(ERROR) << "Invalid pauseAudio.";
    return;
  }
  if (!IsConnected()) {
    return;
  }
  protocol::AudioControl audio_control;
  audio_control.set_enable(!pause);
  client_->host_stub()->ControlAudio(audio_control);
}
