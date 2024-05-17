void ExtensionInstallPrompt::SetIcon(const SkBitmap* image) {
  if (image)
    icon_ = *image;
  else
    icon_ = SkBitmap();
  if (icon_.empty()) {
    icon_ = GetDefaultIconBitmapForMaxScaleFactor(
        extension_ ? extension_->is_app() : false);
  }
}
