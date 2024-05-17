void BackendImpl::PrepareForRestart() {
  if (!(user_flags_ & kMask))
    mask_ = 0;

  if (!(user_flags_ & kNewEviction))
    new_eviction_ = false;

  disabled_ = true;
  data_->header.crash = 0;
  index_->Flush();
  index_ = NULL;
  data_ = NULL;
  block_files_.CloseFiles();
  rankings_.Reset();
  init_ = false;
  restarted_ = true;
}
