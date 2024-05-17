ExtensionInstallDialogView::~ExtensionInstallDialogView() {
  if (!handled_result_ && !done_callback_.is_null()) {
    base::ResetAndReturn(&done_callback_)
        .Run(ExtensionInstallPrompt::Result::USER_CANCELED);
  }
}
