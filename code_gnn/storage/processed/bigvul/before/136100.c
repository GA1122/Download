void PopupHeaderView::SetIdentityStatus(const base::string16& status,
                                        SkColor text_color) {
  status_->SetText(status);
  status_->SetEnabledColor(text_color);
}
