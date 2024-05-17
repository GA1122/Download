PasswordPopupSuggestionView::PasswordPopupSuggestionView(
    AutofillPopupViewNativeViews* popup_view,
    int line_number,
    int frontend_id)
    : AutofillPopupSuggestionView(popup_view, line_number, frontend_id) {
  origin_ = popup_view_->controller()->GetElidedLabelAt(line_number_);
  masked_password_ =
      popup_view_->controller()->GetSuggestionAt(line_number_).additional_label;
}
