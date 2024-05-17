void RenderMessageFilter::OnGetAudioHardwareConfig(
    int* output_buffer_size, int* output_sample_rate, int* input_sample_rate,
    media::ChannelLayout* input_channel_layout) {
  *output_buffer_size = media::GetAudioHardwareBufferSize();
  *output_sample_rate = media::GetAudioHardwareSampleRate();

  *input_sample_rate = media::GetAudioInputHardwareSampleRate(
      media::AudioManagerBase::kDefaultDeviceId);
  *input_channel_layout = media::GetAudioInputHardwareChannelLayout(
      media::AudioManagerBase::kDefaultDeviceId);
}
