void GpuCommandBufferStub::PutChanged() {
  FastSetActiveURL(active_url_, active_url_hash_);
  scheduler_->PutChanged();
}
