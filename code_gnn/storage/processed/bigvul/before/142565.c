void ShelfWidget::OnWidgetActivationChanged(views::Widget* widget,
                                            bool active) {
  if (active) {
    if (activated_from_overflow_bubble_) {
      activated_from_overflow_bubble_ = false;
      return;
    }
    delegate_view_->SetPaneFocusAndFocusDefault();
  } else {
    delegate_view_->GetFocusManager()->ClearFocus();
  }
}
