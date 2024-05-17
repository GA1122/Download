bool BackgroundLoaderOffliner::HandleTimeout(int64_t request_id) {
  if (pending_request_) {
    DCHECK(request_id == pending_request_->request_id());
    if (is_low_bar_met_ && (pending_request_->started_attempt_count() + 1 >=
                                policy_->GetMaxStartedTries() ||
                            pending_request_->completed_attempt_count() + 1 >=
                                policy_->GetMaxCompletedTries())) {
      if (save_state_ == NONE) {
        did_snapshot_on_last_retry_ = true;
        StartSnapshot();
      }
      return true;
    }
  }
  return false;
}
