void ClassicPendingScript::AdvanceReadyState(ReadyState new_ready_state) {
  switch (ready_state_) {
    case kWaitingForResource:
      CHECK_EQ(new_ready_state, kWaitingForStreaming);
      break;
    case kWaitingForStreaming:
      CHECK(new_ready_state == kReady || new_ready_state == kErrorOccurred);
      break;
    case kReady:
      CHECK_EQ(new_ready_state, kReadyStreaming);
      break;
    case kReadyStreaming:
      CHECK_EQ(new_ready_state, kReady);
      break;
    case kErrorOccurred:
      NOTREACHED();
      break;
  }

  bool old_is_ready = IsReady();
  ready_state_ = new_ready_state;

  if (IsReady() && !old_is_ready && IsWatchingForLoad())
    Client()->PendingScriptFinished(this);

  if (IsCurrentlyStreaming()) {
    if (ready_state_ == kReady || ready_state_ == kErrorOccurred) {
      base::OnceClosure done = std::move(streamer_done_);
      if (done)
        std::move(done).Run();
      is_currently_streaming_ = false;
    }
  }


  if (IsCurrentlyStreaming()) {
    DCHECK(streamer_);
    DCHECK(!streamer_->IsFinished());
  }

  DCHECK_EQ(IsCurrentlyStreaming(), streamer_ && !streamer_->IsFinished());
  DCHECK_EQ(IsCurrentlyStreaming(),
            ready_state_ == kReadyStreaming ||
                (streamer_ && (ready_state_ == kWaitingForResource ||
                               ready_state_ == kWaitingForStreaming)));
  DCHECK(IsCurrentlyStreaming() || !streamer_done_);
}
