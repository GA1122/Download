gfx::Image* SessionCrashedInfoBarDelegate::GetIcon() const {
  return &ResourceBundle::GetSharedInstance().GetNativeImageNamed(
      IDR_INFOBAR_RESTORE_SESSION);
}
