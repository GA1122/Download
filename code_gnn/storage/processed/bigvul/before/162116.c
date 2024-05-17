void RenderProcessHostImpl::OnMediaStreamAdded() {
  ++media_stream_count_;
  UpdateProcessPriority();
}
