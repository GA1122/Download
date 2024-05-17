 void ChromeInvalidationClient::Stop() {
   DCHECK(CalledOnValidThread());
   if (!invalidation_client_.get()) {
    return;
  }

  registration_manager_.reset();
  chrome_system_resources_.Stop();
  invalidation_client_->Stop();

  invalidation_client_.reset();
  listener_ = NULL;

  invalidation_state_tracker_.Reset();
  max_invalidation_versions_.clear();
  ticl_state_ = DEFAULT_NOTIFICATION_ERROR;
  push_client_state_ = DEFAULT_NOTIFICATION_ERROR;
}
