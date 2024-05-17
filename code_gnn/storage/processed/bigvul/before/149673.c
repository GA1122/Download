void SaveCardBubbleControllerImpl::DidFinishNavigation(
    content::NavigationHandle* navigation_handle) {
  if (!navigation_handle->IsInMainFrame() || !navigation_handle->HasCommitted())
    return;

  if (save_card_callback_.is_null())
    return;

  if (navigation_handle->IsSameDocument())
    return;

  if (Elapsed() < base::TimeDelta::FromSeconds(kSurviveNavigationSeconds))
    return;

  save_card_callback_.Reset();
  if (save_card_bubble_view_) {
    save_card_bubble_view_->Hide();
    OnBubbleClosed();

    AutofillMetrics::LogSaveCardPromptMetric(
        AutofillMetrics::SAVE_CARD_PROMPT_END_NAVIGATION_SHOWING, is_uploading_,
        is_reshow_,
        pref_service_->GetInteger(
            prefs::kAutofillAcceptSaveCreditCardPromptState));
  } else {
    UpdateIcon();

    AutofillMetrics::LogSaveCardPromptMetric(
        AutofillMetrics::SAVE_CARD_PROMPT_END_NAVIGATION_HIDDEN, is_uploading_,
        is_reshow_,
        pref_service_->GetInteger(
            prefs::kAutofillAcceptSaveCreditCardPromptState));
  }
}
