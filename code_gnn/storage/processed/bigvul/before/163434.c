gfx::Size OmniboxViewViews::GetMinimumSize() const {
  const int kMinCharacters = 10;
  return gfx::Size(
      GetFontList().GetExpectedTextWidth(kMinCharacters) + GetInsets().width(),
      GetPreferredSize().height());
}
