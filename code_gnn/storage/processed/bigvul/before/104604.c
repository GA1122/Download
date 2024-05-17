const SkBitmap& Extension::GetDefaultIcon(bool is_app) {
  if (is_app) {
    return *ResourceBundle::GetSharedInstance().GetBitmapNamed(
        IDR_APP_DEFAULT_ICON);
  } else {
    return *ResourceBundle::GetSharedInstance().GetBitmapNamed(
        IDR_EXTENSION_DEFAULT_ICON);
  }
}
