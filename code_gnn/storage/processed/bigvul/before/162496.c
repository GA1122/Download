void ClassicPendingScript::CancelStreaming() {
  if (!streamer_)
    return;

  streamer_->Cancel();
  streamer_ = nullptr;
  streamer_done_.Reset();
  is_currently_streaming_ = false;
  DCHECK(!IsCurrentlyStreaming());
}
