void PasswordAutofillManager::OnShowManualFallbackSuggestion(
    base::i18n::TextDirection text_direction,
    const gfx::RectF& bounds) {
  if (!autofill_client_ || !ShouldShowManualFallbackForPreLollipop(
                               autofill_client_->GetSyncService()))
    return;
  if (!password_client_ ||
      !password_client_->IsFillingFallbackEnabledForCurrentPage())
    return;
  std::vector<autofill::Suggestion> suggestions;
  autofill::Suggestion all_saved_passwords(
      l10n_util::GetStringUTF8(IDS_AUTOFILL_SHOW_ALL_SAVED_FALLBACK),
      std::string(), std::string(),
      autofill::POPUP_ITEM_ID_ALL_SAVED_PASSWORDS_ENTRY);
  suggestions.push_back(all_saved_passwords);

  show_all_saved_passwords_shown_context_ =
      metrics_util::SHOW_ALL_SAVED_PASSWORDS_CONTEXT_MANUAL_FALLBACK;
  metrics_util::LogContextOfShowAllSavedPasswordsShown(
      show_all_saved_passwords_shown_context_);

  if (base::FeatureList::IsEnabled(
          password_manager::features::kEnableManualFallbacksGeneration) &&
      password_manager_util::GetPasswordSyncState(
          autofill_client_->GetSyncService()) == SYNCING_NORMAL_ENCRYPTION) {
    AddSimpleSuggestionWithSeparatorOnTop(
        IDS_AUTOFILL_GENERATE_PASSWORD_FALLBACK,
        autofill::POPUP_ITEM_ID_GENERATE_PASSWORD_ENTRY, &suggestions);
  }
  autofill_client_->ShowAutofillPopup(bounds, text_direction, suggestions,
                                      weak_ptr_factory_.GetWeakPtr());
}
