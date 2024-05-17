  void ChromeInvalidationClient::Ready(
      invalidation::InvalidationClient* client) {
//   DCHECK(CalledOnValidThread());
//   DCHECK_EQ(client, invalidation_client_.get());
    ticl_state_ = NO_NOTIFICATION_ERROR;
    EmitStateChange();
  registration_manager_->SetRegisteredIds(registered_ids_);
//   registration_manager_->UpdateRegisteredIds(registered_ids_);
  }