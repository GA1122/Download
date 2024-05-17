void Job::ReleaseCallback() {
  CheckIsOnOriginThread();
  DCHECK(!callback_.is_null());
  CHECK_GT(*params_->num_outstanding_callbacks, 0);
  (*params_->num_outstanding_callbacks)--;
  callback_.Reset();

  user_results_ = NULL;
}
