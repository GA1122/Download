gfx::Size AutofillDialogViews::GetMinimumSignInViewSize() const {
  return gfx::Size(GetDialogClientView()->size().width() - GetInsets().width(),
                   kMinimumContentsHeight);
}
