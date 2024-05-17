void AudioHandler::Initialize() {
  DCHECK_EQ(new_channel_count_mode_, channel_count_mode_);
  DCHECK_EQ(new_channel_interpretation_, channel_interpretation_);

  is_initialized_ = true;
}
