void DefaultAudioDestinationHandler::StopPlatformDestination() {
  if (platform_destination_->IsPlaying()) {
    platform_destination_->Stop();
  }
}
