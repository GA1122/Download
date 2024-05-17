void RenderMessageFilter::OnGetHardwareInputSampleRate(int* sample_rate) {
  *sample_rate = media::GetAudioInputHardwareSampleRate(
      media::AudioManagerBase::kDefaultDeviceId);
}
