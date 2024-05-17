void AudioHandler::UpdateChannelCountMode() {
  channel_count_mode_ = new_channel_count_mode_;
  UpdateChannelsForInputs();
}
