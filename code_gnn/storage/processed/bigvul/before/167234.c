int ConfirmInfoBar::NonLabelWidth() const {
  ChromeLayoutProvider* layout_provider = ChromeLayoutProvider::Get();

  const int label_spacing = layout_provider->GetDistanceMetric(
      views::DISTANCE_RELATED_LABEL_HORIZONTAL);
  const int button_spacing = layout_provider->GetDistanceMetric(
      views::DISTANCE_RELATED_BUTTON_HORIZONTAL);

  int width = (label_->text().empty() || (!ok_button_ && !cancel_button_))
                  ? 0
                  : label_spacing;
  if (ok_button_)
    width += ok_button_->width() + (cancel_button_ ? button_spacing : 0);
  width += cancel_button_ ? cancel_button_->width() : 0;
  return width + ((link_->text().empty() || !width) ? 0 : label_spacing);
}
