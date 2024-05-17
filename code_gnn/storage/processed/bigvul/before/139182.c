void RenderProcessHostImpl::OnAudioStreamRemoved() {
  DCHECK_GT(audio_stream_count_, 0);
  --audio_stream_count_;
  UpdateProcessPriority();
}
