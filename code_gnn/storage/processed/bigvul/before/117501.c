AudioHandler::AudioHandler()
    : mixer_(new AudioMixerAlsa()) {
  mixer_->Init();
}
