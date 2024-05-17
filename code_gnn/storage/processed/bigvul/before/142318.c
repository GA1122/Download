void ChromePasswordManagerClient::PasswordWasAutofilled(
    const std::map<base::string16, const autofill::PasswordForm*>& best_matches,
    const GURL& origin,
    const std::vector<const autofill::PasswordForm*>* federated_matches) const {
#if defined(OS_ANDROID)
  if (!PasswordAccessoryController::AllowedForWebContents(web_contents())) {
    return;   
  }

  PasswordAccessoryController::GetOrCreate(web_contents())
      ->SavePasswordsForOrigin(best_matches, url::Origin::Create(origin));
#else   
  PasswordsClientUIDelegate* manage_passwords_ui_controller =
      PasswordsClientUIDelegateFromWebContents(web_contents());
  manage_passwords_ui_controller->OnPasswordAutofilled(best_matches, origin,
                                                       federated_matches);
#endif
}
