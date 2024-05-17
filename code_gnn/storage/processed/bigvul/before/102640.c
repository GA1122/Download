void CloudPolicyController::SendPolicyRequest() {
  backend_.reset(service_->CreateBackend());
  DCHECK(!data_store_->device_token().empty());
  em::DevicePolicyRequest policy_request;
  em::PolicyFetchRequest* fetch_request = policy_request.add_request();
  fetch_request->set_signature_type(em::PolicyFetchRequest::SHA1_RSA);
  fetch_request->set_policy_type(data_store_->policy_type());
  if (!cache_->is_unmanaged() &&
      !cache_->last_policy_refresh_time().is_null()) {
    base::TimeDelta timestamp =
        cache_->last_policy_refresh_time() - base::Time::UnixEpoch();
    fetch_request->set_timestamp(timestamp.InMilliseconds());
  }
  int key_version = 0;
  if (cache_->GetPublicKeyVersion(&key_version))
    fetch_request->set_public_key_version(key_version);

  backend_->ProcessPolicyRequest(data_store_->device_token(),
                                 data_store_->device_id(),
                                 policy_request, this);
}
