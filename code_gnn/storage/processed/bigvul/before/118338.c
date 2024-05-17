AutofillDialogViews::~AutofillDialogViews() {
  HideErrorBubble();
  DCHECK(!window_);
}
