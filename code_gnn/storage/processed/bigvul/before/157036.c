void MultibufferDataSource::OnBufferingHaveEnough(bool always_cancel) {
  DCHECK(render_task_runner_->BelongsToCurrentThread());
  url_data_and_loading_state_.SetLoadingState(
      UrlData::UrlDataWithLoadingState::LoadingState::kIdle);
  if (reader_ && (always_cancel || (preload_ == METADATA &&
                                    !media_has_played_ && !IsStreaming()))) {
    cancel_on_defer_ = true;
    if (!loading_) {
      base::AutoLock auto_lock(lock_);
      if (read_op_) {
        return;
      }
      reader_.reset(nullptr);
    }
  }
}
