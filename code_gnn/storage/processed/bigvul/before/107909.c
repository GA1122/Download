InfoBar* ExtensionInfoBarDelegate::CreateInfoBar() {
  return new ExtensionInfoBar(this);
}
