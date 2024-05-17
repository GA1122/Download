void CredentialManagerImpl::Store(const CredentialInfo& credential,
                                  StoreCallback callback) {
  DCHECK_NE(CredentialType::CREDENTIAL_TYPE_EMPTY, credential.type);

  if (password_manager_util::IsLoggingActive(client_)) {
    CredentialManagerLogger(client_->GetLogManager())
        .LogStoreCredential(GetLastCommittedURL(), credential.type);
  }

  std::move(callback).Run();

  if (!client_->IsSavingAndFillingEnabledForCurrentPage() ||
      !client_->OnCredentialManagerUsed())
    return;

  client_->NotifyStorePasswordCalled();

  GURL origin = GetLastCommittedURL().GetOrigin();
  std::unique_ptr<autofill::PasswordForm> form(
      CreatePasswordFormFromCredentialInfo(credential, origin));

  std::unique_ptr<autofill::PasswordForm> observed_form =
      CreateObservedPasswordFormFromOrigin(origin);
  auto form_fetcher = std::make_unique<FormFetcherImpl>(
      PasswordStore::FormDigest(*observed_form), client_, false, false);
  form_manager_ = std::make_unique<CredentialManagerPasswordFormManager>(
      client_, *observed_form, std::move(form), this, nullptr,
      std::move(form_fetcher));
  form_manager_->Init(nullptr);
}
