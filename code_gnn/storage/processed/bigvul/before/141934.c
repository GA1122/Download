void AutofillPopupItemView::OnMouseExited(const ui::MouseEvent& event) {
  AutofillPopupController* controller = popup_view_->controller();
  if (controller)
    controller->SelectionCleared();
}
