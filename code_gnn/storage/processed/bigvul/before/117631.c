gfx::Image* AutolaunchInfoBarDelegate::GetIcon() const {
  return &ResourceBundle::GetSharedInstance().GetNativeImageNamed(
      IDR_PRODUCT_LOGO_32);
}
