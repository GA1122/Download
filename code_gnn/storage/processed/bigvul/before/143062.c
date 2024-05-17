void DefaultAudioDestinationHandler::CreatePlatformDestination() {
  platform_destination_ =
      AudioDestination::Create(*this, ChannelCount(), latency_hint_);
}
