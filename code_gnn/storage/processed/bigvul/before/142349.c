void PasswordAccessoryControllerImpl::OnOptionSelected(
    const base::string16& selected_option) const {
  if (selected_option ==
      l10n_util::GetStringUTF16(
          IDS_PASSWORD_MANAGER_ACCESSORY_ALL_PASSWORDS_LINK)) {
    UMA_HISTOGRAM_ENUMERATION("KeyboardAccessory.AccessoryActionSelected",
                              metrics::AccessoryAction::MANAGE_PASSWORDS,
                              metrics::AccessoryAction::COUNT);
    chrome::android::PreferencesLauncher::ShowPasswordSettings(
        web_contents_,
        password_manager::ManagePasswordsReferrer::kPasswordsAccessorySheet);
  }
}
