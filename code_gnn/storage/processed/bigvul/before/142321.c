bool ChromePasswordManagerClient::PromptUserToChooseCredentials(
    std::vector<std::unique_ptr<autofill::PasswordForm>> local_forms,
    const GURL& origin,
    const CredentialsCallback& callback) {
  CredentialsCallback intercept =
      base::Bind(&PasswordManagerClientHelper::OnCredentialsChosen,
                 base::Unretained(&helper_), callback, local_forms.size() == 1);
#if defined(OS_ANDROID)
  AccountChooserDialogAndroid* acccount_chooser_dialog =
      new AccountChooserDialogAndroid(web_contents(), std::move(local_forms),
                                      origin, intercept);
  acccount_chooser_dialog->ShowDialog();
  return true;
#else
  return PasswordsClientUIDelegateFromWebContents(web_contents())
      ->OnChooseCredentials(std::move(local_forms), origin, intercept);
#endif
}
