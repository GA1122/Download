double DefaultAudioDestinationHandler::SampleRate() const {
  return platform_destination_ ? platform_destination_->SampleRate() : 0;
}
