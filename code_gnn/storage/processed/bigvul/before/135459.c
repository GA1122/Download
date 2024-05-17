void PasswordAutofillManager::DidAcceptSuggestion(const base::string16& value,
                                                  int identifier,
                                                  int position) {
  autofill_client_->ExecuteCommand(identifier);
  if (identifier == autofill::POPUP_ITEM_ID_GENERATE_PASSWORD_ENTRY) {
    password_manager_driver_->UserSelectedManualGenerationOption();
  } else if (identifier !=
                 autofill::POPUP_ITEM_ID_HTTP_NOT_SECURE_WARNING_MESSAGE &&
             identifier != autofill::POPUP_ITEM_ID_ALL_SAVED_PASSWORDS_ENTRY) {
    bool success =
        FillSuggestion(form_data_key_, GetUsernameFromSuggestion(value));
    DCHECK(success);
  }

  if (identifier == autofill::POPUP_ITEM_ID_ALL_SAVED_PASSWORDS_ENTRY) {
    DCHECK_NE(show_all_saved_passwords_shown_context_,
              metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_NONE);

    metrics_util::LogContextOfShowAllSavedPasswordsAccepted(
        show_all_saved_passwords_shown_context_);

    if (password_client_) {
      using UserAction =
          password_manager::PasswordManagerMetricsRecorder::PageLevelUserAction;
      switch (show_all_saved_passwords_shown_context_) {
        case metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_PASSWORD:
          password_client_->GetMetricsRecorder().RecordPageLevelUserAction(
              UserAction::kShowAllPasswordsWhileSomeAreSuggested);
          break;
        case metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_MANUAL_FALLBACK:
          password_client_->GetMetricsRecorder().RecordPageLevelUserAction(
              UserAction::kShowAllPasswordsWhileNoneAreSuggested);
          break;
        case metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_CONTEXT_MENU:
        case metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_NONE:
        case metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_COUNT:
          NOTREACHED();
      }
    }
  }

  autofill_client_->HideAutofillPopup();
}
