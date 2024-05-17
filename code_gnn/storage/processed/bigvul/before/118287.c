void AutofillDialogViews::OnWillChangeFocus(
    views::View* focused_before,
    views::View* focused_now) {
  delegate_->FocusMoved();
  HideErrorBubble();
}
