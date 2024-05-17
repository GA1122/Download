void Textfield::UpdateBorder() {
  auto border = std::make_unique<views::FocusableBorder>();
  const LayoutProvider* provider = LayoutProvider::Get();
  border->SetInsets(
      provider->GetDistanceMetric(DISTANCE_CONTROL_VERTICAL_TEXT_PADDING),
      provider->GetDistanceMetric(DISTANCE_TEXTFIELD_HORIZONTAL_TEXT_PADDING));
  if (invalid_)
    border->SetColorId(ui::NativeTheme::kColorId_AlertSeverityHigh);
  View::SetBorder(std::move(border));
}
