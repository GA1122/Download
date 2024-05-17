gfx::Size AutofillDialogViews::SuggestedButton::GetPreferredSize() const {
  ui::ResourceBundle& rb = ui::ResourceBundle::GetSharedInstance();
  gfx::Size size = rb.GetImageNamed(ResourceIDForState()).Size();
  const gfx::Insets insets = GetInsets();
  size.Enlarge(insets.width(), insets.height());
  return size;
}
