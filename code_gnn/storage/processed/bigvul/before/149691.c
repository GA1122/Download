void SaveCardBubbleControllerImpl::ShowBubble() {
  DCHECK(!save_card_callback_.is_null());
  DCHECK(!save_card_bubble_view_);

  UpdateIcon();

  Browser* browser = chrome::FindBrowserWithWebContents(web_contents());
  save_card_bubble_view_ = browser->window()->ShowSaveCreditCardBubble(
      web_contents(), this, is_reshow_);
  DCHECK(save_card_bubble_view_);

  UpdateIcon();

  timer_.reset(new base::ElapsedTimer());

  AutofillMetrics::LogSaveCardPromptMetric(
      AutofillMetrics::SAVE_CARD_PROMPT_SHOWN, is_uploading_, is_reshow_,
      pref_service_->GetInteger(
          prefs::kAutofillAcceptSaveCreditCardPromptState));
}
