gfx::ImageSkia BrowserView::GetOTRAvatarIcon() const {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  const gfx::ImageSkia* otr_avatar =
      rb.GetNativeImageNamed(GetOTRIconResourceID()).ToImageSkia();
  return *otr_avatar;
}
