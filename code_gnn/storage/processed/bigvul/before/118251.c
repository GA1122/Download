void AutofillDialogViews::HideSignIn() {
  sign_in_web_view_->SetWebContents(NULL);

  if (delegate_->ShouldShowSpinner()) {
    UpdateAccountChooser();
  } else {
    ShowDialogInMode(DETAIL_INPUT);
    InvalidateLayout();
  }
  DCHECK(!sign_in_web_view_->visible());

  ContentsPreferredSizeChanged();
}
