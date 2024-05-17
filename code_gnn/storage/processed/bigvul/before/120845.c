void ConfirmEmailDialogDelegate::OnCanceled() {
  base::ResetAndReturn(&callback_).Run(
      IDS_ONE_CLICK_SIGNIN_CONFIRM_EMAIL_DIALOG_CANCEL_BUTTON);
}
