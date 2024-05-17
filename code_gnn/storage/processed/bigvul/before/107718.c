bool AudioHandler::IsMute() {
  if (!VerifyMixerConnection())
    return false;

  return mixer_->IsMute();
}
