void CredentialManagerImpl::SendPasswordForm(
    const SendCredentialCallback& send_callback,
    CredentialMediationRequirement mediation,
    const autofill::PasswordForm* form) {
  CredentialInfo info;
  if (form) {
    password_manager::CredentialType type_to_return =
        form->federation_origin.unique()
            ? CredentialType::CREDENTIAL_TYPE_PASSWORD
            : CredentialType::CREDENTIAL_TYPE_FEDERATED;
    info = CredentialInfo(*form, type_to_return);
    if (PasswordStore* store = GetPasswordStore()) {
      if (form->skip_zero_click && IsZeroClickAllowed()) {
        autofill::PasswordForm update_form = *form;
        update_form.skip_zero_click = false;
        store->UpdateLogin(update_form);
      }
    }
    base::RecordAction(
        base::UserMetricsAction("CredentialManager_AccountChooser_Accepted"));
    metrics_util::LogCredentialManagerGetResult(
        metrics_util::CREDENTIAL_MANAGER_GET_ACCOUNT_CHOOSER, mediation);
  } else {
    base::RecordAction(
        base::UserMetricsAction("CredentialManager_AccountChooser_Dismissed"));
    metrics_util::LogCredentialManagerGetResult(
        metrics_util::CREDENTIAL_MANAGER_GET_NONE, mediation);
  }
  SendCredential(send_callback, info);
}
