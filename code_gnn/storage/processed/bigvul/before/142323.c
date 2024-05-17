bool ChromePasswordManagerClient::PromptUserToSaveOrUpdatePassword(
    std::unique_ptr<password_manager::PasswordFormManagerForUI> form_to_save,
    bool update_password) {
  if (!CanShowBubbleOnURL(web_contents()->GetLastCommittedURL()))
    return false;

#if !defined(OS_ANDROID)
  PasswordsClientUIDelegate* manage_passwords_ui_controller =
      PasswordsClientUIDelegateFromWebContents(web_contents());
  if (update_password) {
    manage_passwords_ui_controller->OnUpdatePasswordSubmitted(
        std::move(form_to_save));
  } else {
    manage_passwords_ui_controller->OnPasswordSubmitted(
        std::move(form_to_save));
  }
#else
  if (form_to_save->IsBlacklisted())
    return false;

  if (update_password) {
    UpdatePasswordInfoBarDelegate::Create(web_contents(),
                                          std::move(form_to_save));
  } else {
    SavePasswordInfoBarDelegate::Create(web_contents(),
                                        std::move(form_to_save));
  }
#endif   
  return true;
}
