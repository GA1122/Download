CredentialManagerImpl::CredentialManagerImpl(PasswordManagerClient* client)
    : client_(client) {
  auto_signin_enabled_.Init(prefs::kCredentialsEnableAutosignin,
                            client_->GetPrefs());
}
