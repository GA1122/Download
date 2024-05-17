void RenderMessageFilter::OnGetHardwareSampleRate(int* sample_rate) {
  *sample_rate = media::GetAudioHardwareSampleRate();
}
