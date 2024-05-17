void ExtensionInstallPrompt::OnInstallSuccess(const Extension* extension,
                                              SkBitmap* icon) {
  extension_ = extension;
  SetIcon(icon);

  install_ui_->OnInstallSuccess(extension, &icon_);
}
