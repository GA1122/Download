int CardUnmaskPromptViews::GetHeightForWidth(int width) const {
  if (!has_children())
    return 0;
  const gfx::Insets insets = GetInsets();
  return main_contents_->GetHeightForWidth(width - insets.width()) +
         insets.height();
}
