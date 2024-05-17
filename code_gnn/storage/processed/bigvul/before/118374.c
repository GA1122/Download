void CardUnmaskPromptViews::SetRetriableErrorMessage(
    const base::string16& message) {
  if (message.empty()) {
    error_label_->SetMultiLine(false);
    error_label_->SetText(base::ASCIIToUTF16(" "));
  } else {
    error_label_->SetMultiLine(true);
    error_label_->SetText(message);
  }

  if (GetWidget() && controller_->GetWebContents()) {
    constrained_window::UpdateWebContentsModalDialogPosition(
        GetWidget(), web_modal::WebContentsModalDialogManager::FromWebContents(
                         controller_->GetWebContents())
                         ->delegate()
                         ->GetWebContentsModalDialogHost());
  }
}
