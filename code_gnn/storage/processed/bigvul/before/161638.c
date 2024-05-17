void AudioHandler::SetInternalChannelCountMode(ChannelCountMode mode) {
  channel_count_mode_ = mode;
  new_channel_count_mode_ = mode;
}
