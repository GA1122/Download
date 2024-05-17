void AutofillDialogViews::UpdateErrorBubble() {
  if (!delegate_->ShouldShowErrorBubble())
    HideErrorBubble();
}
