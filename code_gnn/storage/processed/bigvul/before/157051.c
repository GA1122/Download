void MultibufferDataSource::UpdateLoadingState_Locked(bool force_loading) {
  DVLOG(1) << __func__;
  lock_.AssertAcquired();
  if (assume_fully_buffered())
    return;
  bool is_loading = !!reader_ && reader_->IsLoading();
  if (force_loading || is_loading != loading_) {
    bool loading = is_loading || force_loading;

    if (!loading && cancel_on_defer_) {
      if (read_op_) {
        return;
      }
      reader_.reset(nullptr);
    }

    loading_ = loading;

    downloading_cb_.Run(loading_);
  }
}
