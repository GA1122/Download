void RenderProcessHostImpl::OnMediaStreamRemoved() {
  DCHECK_GT(media_stream_count_, 0);
  --media_stream_count_;
  UpdateProcessPriority();
}
