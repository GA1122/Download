void CloudPolicyController::OnCredentialsChanged() {
  if (data_store_->device_token().empty()) {
    notifier_->Inform(CloudPolicySubsystem::UNENROLLED,
                      CloudPolicySubsystem::NO_DETAILS,
                      PolicyNotifier::POLICY_CONTROLLER);
    effective_policy_refresh_error_delay_ms_ =
        kPolicyRefreshErrorDelayInMilliseconds;
    SetState(STATE_TOKEN_UNAVAILABLE);
  }
}
