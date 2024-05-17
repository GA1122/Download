int ConfirmInfoBar::ContentMinimumWidth() const {
  return label_->GetMinimumSize().width() + link_->GetMinimumSize().width() +
      NonLabelWidth();
}
