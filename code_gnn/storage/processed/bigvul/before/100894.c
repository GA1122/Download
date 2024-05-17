void ExtensionInstallUI::OnImageLoaded(
    SkBitmap* image, const ExtensionResource& resource, int index) {
  SetIcon(image);

  switch (prompt_type_) {
    case PERMISSIONS_PROMPT:
    case RE_ENABLE_PROMPT:
    case INSTALL_PROMPT: {
      content::NotificationService* service =
          content::NotificationService::current();
      service->Notify(chrome::NOTIFICATION_EXTENSION_WILL_SHOW_CONFIRM_DIALOG,
          content::Source<ExtensionInstallUI>(this),
          content::NotificationService::NoDetails());

      Prompt prompt(prompt_type_);
      prompt.SetPermissions(permissions_->GetWarningMessages());
      ShowExtensionInstallDialog(
          profile_, delegate_, extension_, &icon_, prompt);
      break;
    }
    default:
      NOTREACHED() << "Unknown message";
      break;
  }
}
