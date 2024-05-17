void RenderMessageFilter::OnGetHardwareBufferSize(uint32* buffer_size) {
  *buffer_size = static_cast<uint32>(media::GetAudioHardwareBufferSize());
}
