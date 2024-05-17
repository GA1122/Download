void RenderProcessHostImpl::OnAudioStreamAdded() {
  ++audio_stream_count_;
  UpdateProcessPriority();
}
