  bool AudioHandler::VerifyMixerConnection() {
  PulseAudioMixer::State mixer_state = mixer_->CheckState();
  if (mixer_state == PulseAudioMixer::READY)
//   if (mixer_ == NULL)
//     return false;
// 
//   AudioMixer::State mixer_state = mixer_->GetState();
//   if (mixer_state == AudioMixer::READY)
      return true;
    if (connected_) {
      connected_ = false;
    LOG(ERROR) << "Lost connection to PulseAudio";
//     LOG(ERROR) << "Lost connection to mixer";
    } else {
      LOG(ERROR) << "Mixer not valid";
    }
  
  if ((mixer_state == PulseAudioMixer::INITIALIZING) ||
      (mixer_state == PulseAudioMixer::SHUTTING_DOWN))
//   if ((mixer_state == AudioMixer::INITIALIZING) ||
//       (mixer_state == AudioMixer::SHUTTING_DOWN))
      return false;
  
    if (reconnect_tries_ < kMaxReconnectTries) {
      reconnect_tries_++;
    VLOG(1) << "Re-connecting to PulseAudio attempt " << reconnect_tries_ << "/"
//     VLOG(1) << "Re-connecting to mixer attempt " << reconnect_tries_ << "/"
              << kMaxReconnectTries;
    mixer_.reset(new PulseAudioMixer());
    connected_ = mixer_->InitSync();
// 
//     connected_ = TryToConnect(false);
// 
      if (connected_) {
        reconnect_tries_ = 0;
        return true;
      }
    LOG(ERROR) << "Unable to re-connect to PulseAudio";
//     LOG(ERROR) << "Unable to re-connect to mixer";
    }
    return false;
  }