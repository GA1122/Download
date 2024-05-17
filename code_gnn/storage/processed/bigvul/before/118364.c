void CardUnmaskPromptViews::GotVerificationResult(
    const base::string16& error_message,
    bool allow_retry) {
  progress_throbber_->Stop();
  if (error_message.empty()) {
    progress_label_->SetText(l10n_util::GetStringUTF16(
        IDS_AUTOFILL_CARD_UNMASK_VERIFICATION_SUCCESS));
    progress_throbber_->SetChecked(true);
    base::MessageLoop::current()->PostDelayedTask(
        FROM_HERE, base::Bind(&CardUnmaskPromptViews::ClosePrompt,
                              weak_ptr_factory_.GetWeakPtr()),
        controller_->GetSuccessMessageDuration());
  } else {
    overlay_animation_.Reset();
    storage_row_->SetOpacity(1.0);
    progress_overlay_->SetVisible(false);

    if (allow_retry) {
      SetInputsEnabled(true);

      if (!controller_->ShouldRequestExpirationDate())
        cvc_input_->SetInvalid(true);

      SetRetriableErrorMessage(error_message);
    } else {
      permanent_error_label_->SetText(error_message);
      permanent_error_label_->SetVisible(true);
      SetRetriableErrorMessage(base::string16());
    }
    GetDialogClientView()->UpdateDialogButtons();
  }

  Layout();
}
