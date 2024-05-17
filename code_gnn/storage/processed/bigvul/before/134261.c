int OmniboxViewViews::GetTextWidth() const {
  return GetRenderText()->GetContentWidth() + GetInsets().width();
}
