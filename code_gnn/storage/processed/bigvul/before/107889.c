int SaveCCInfoConfirmInfoBar::GetAvailableWidth() const {
  int buttons_area_size = save_button_->GetPreferredSize().width() +
      dont_save_button_->GetPreferredSize().width() + kButtonButtonSpacing +
      kEndOfLabelSpacing;
  return std::max(0, link_->x() - buttons_area_size);
}
