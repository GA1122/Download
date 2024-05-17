void ConfirmEmailDialogDelegate::OnAccepted() {
  base::ResetAndReturn(&callback_).Run(
      IDS_ONE_CLICK_SIGNIN_CONFIRM_EMAIL_DIALOG_OK_BUTTON);
}
