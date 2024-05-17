void ExtensionInstallUI::SetIcon(SkBitmap* image) {
  if (image)
    icon_ = *image;
  else
    icon_ = SkBitmap();
  if (icon_.empty())
    icon_ = Extension::GetDefaultIcon(extension_->is_app());
}
