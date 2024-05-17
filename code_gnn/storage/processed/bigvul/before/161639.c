void AudioHandler::SetInternalChannelInterpretation(
    AudioBus::ChannelInterpretation interpretation) {
  channel_interpretation_ = interpretation;
  new_channel_interpretation_ = interpretation;
}
