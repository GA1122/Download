void CloudPolicyController::OnDeviceTokenChanged() {
  if (data_store_->device_token().empty())
    SetState(STATE_TOKEN_UNAVAILABLE);
  else
    SetState(STATE_TOKEN_VALID);
}
