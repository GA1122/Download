void CredentialManagerImpl::PreventSilentAccess(
    PreventSilentAccessCallback callback) {
  if (password_manager_util::IsLoggingActive(client_)) {
    CredentialManagerLogger(client_->GetLogManager())
        .LogPreventSilentAccess(GetLastCommittedURL());
  }
  std::move(callback).Run();

  PasswordStore* store = GetPasswordStore();
  if (!store || !client_->IsSavingAndFillingEnabledForCurrentPage() ||
      !client_->OnCredentialManagerUsed())
    return;

  if (!pending_require_user_mediation_) {
    pending_require_user_mediation_.reset(
        new CredentialManagerPendingPreventSilentAccessTask(this));
  }
  pending_require_user_mediation_->AddOrigin(GetSynthesizedFormForOrigin());
}
