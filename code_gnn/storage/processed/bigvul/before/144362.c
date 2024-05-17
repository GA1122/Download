base::string16 ExtensionInstallPrompt::Prompt::GetAcceptButtonLabel() const {
  int id = kAcceptButtonIds[type_];

  if (type_ == INSTALL_PROMPT || type_ == INLINE_INSTALL_PROMPT ||
      type_ == DELEGATED_PERMISSIONS_PROMPT) {
    if (extension_->is_app())
      id = IDS_EXTENSION_INSTALL_PROMPT_ACCEPT_BUTTON_APP;
    else if (extension_->is_theme())
      id = IDS_EXTENSION_INSTALL_PROMPT_ACCEPT_BUTTON_THEME;
    else
      id = IDS_EXTENSION_INSTALL_PROMPT_ACCEPT_BUTTON_EXTENSION;
  } else if (type_ == EXTERNAL_INSTALL_PROMPT) {
    if (extension_->is_app())
      id = IDS_EXTENSION_EXTERNAL_INSTALL_PROMPT_ACCEPT_BUTTON_APP;
    else if (extension_->is_theme())
      id = IDS_EXTENSION_EXTERNAL_INSTALL_PROMPT_ACCEPT_BUTTON_THEME;
    else
      id = IDS_EXTENSION_EXTERNAL_INSTALL_PROMPT_ACCEPT_BUTTON_EXTENSION;
  } else if (type_ == POST_INSTALL_PERMISSIONS_PROMPT) {
    if (GetRetainedFileCount() && GetRetainedDeviceCount()) {
      id =
          IDS_EXTENSION_PROMPT_PERMISSIONS_CLEAR_RETAINED_FILES_AND_DEVICES_BUTTON;
    } else if (GetRetainedFileCount()) {
      id = IDS_EXTENSION_PROMPT_PERMISSIONS_CLEAR_RETAINED_FILES_BUTTON;
    } else if (GetRetainedDeviceCount()) {
      id = IDS_EXTENSION_PROMPT_PERMISSIONS_CLEAR_RETAINED_DEVICES_BUTTON;
    }
  } else if (type_ == REMOTE_INSTALL_PROMPT) {
    if (extension_->is_app())
      id = IDS_EXTENSION_PROMPT_REMOTE_INSTALL_BUTTON_APP;
    else
      id = IDS_EXTENSION_PROMPT_REMOTE_INSTALL_BUTTON_EXTENSION;
  } else if (type_ == REPAIR_PROMPT) {
    if (extension_->is_app())
      id = IDS_EXTENSION_PROMPT_REPAIR_BUTTON_APP;
    else
      id = IDS_EXTENSION_PROMPT_REPAIR_BUTTON_EXTENSION;
  }
  return id ? l10n_util::GetStringUTF16(id) : base::string16();
}
