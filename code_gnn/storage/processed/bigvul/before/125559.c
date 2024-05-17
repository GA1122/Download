void RenderMessageFilter::OnGetHardwareInputChannelLayout(
    media::ChannelLayout* layout) {
  *layout = media::GetAudioInputHardwareChannelLayout(
      media::AudioManagerBase::kDefaultDeviceId);
}
