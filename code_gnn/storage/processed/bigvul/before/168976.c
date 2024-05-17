void CredentialManagerImpl::Get(CredentialMediationRequirement mediation,
                                bool include_passwords,
                                const std::vector<GURL>& federations,
                                GetCallback callback) {
  using metrics_util::LogCredentialManagerGetResult;

  PasswordStore* store = GetPasswordStore();
  if (password_manager_util::IsLoggingActive(client_)) {
    CredentialManagerLogger(client_->GetLogManager())
        .LogRequestCredential(GetLastCommittedURL(), mediation, federations);
  }
  if (pending_request_ || !store) {
    std::move(callback).Run(
        pending_request_ ? CredentialManagerError::PENDING_REQUEST
                         : CredentialManagerError::PASSWORDSTOREUNAVAILABLE,
        base::nullopt);
    LogCredentialManagerGetResult(metrics_util::CREDENTIAL_MANAGER_GET_REJECTED,
                                  mediation);
    return;
  }

  if (!client_->IsFillingEnabledForCurrentPage() ||
      !client_->OnCredentialManagerUsed()) {
    std::move(callback).Run(CredentialManagerError::SUCCESS, CredentialInfo());
    LogCredentialManagerGetResult(metrics_util::CREDENTIAL_MANAGER_GET_NONE,
                                  mediation);
    return;
  }
  if (mediation == CredentialMediationRequirement::kSilent &&
      !IsZeroClickAllowed()) {
    std::move(callback).Run(CredentialManagerError::SUCCESS, CredentialInfo());
    LogCredentialManagerGetResult(
        metrics_util::CREDENTIAL_MANAGER_GET_NONE_ZERO_CLICK_OFF, mediation);
    return;
  }

  pending_request_.reset(new CredentialManagerPendingRequestTask(
      this, base::Bind(&RunGetCallback, base::Passed(&callback)), mediation,
      include_passwords, federations));
  GetPasswordStore()->GetLogins(GetSynthesizedFormForOrigin(),
                                pending_request_.get());
}
