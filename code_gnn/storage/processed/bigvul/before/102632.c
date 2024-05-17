void CloudPolicyController::FetchToken() {
  if (data_store_->token_cache_loaded() &&
      !data_store_->user_name().empty() &&
      data_store_->has_auth_token()) {
    if (CanBeInManagedDomain(data_store_->user_name())) {
      data_store_->set_device_id(guid::GenerateGUID());
      token_fetcher_->FetchToken();
    } else {
      SetState(STATE_TOKEN_UNMANAGED);
    }
  }
}
