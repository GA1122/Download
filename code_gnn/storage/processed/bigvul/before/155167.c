void OmniboxViewViews::SetEmphasis(bool emphasize, const gfx::Range& range) {
  SkColor color = location_bar_view_->GetColor(
      emphasize ? OmniboxPart::LOCATION_BAR_TEXT_DEFAULT
                : OmniboxPart::LOCATION_BAR_TEXT_DIMMED);
  if (range.IsValid())
    ApplyColor(color, range);
  else
    SetColor(color);
}
