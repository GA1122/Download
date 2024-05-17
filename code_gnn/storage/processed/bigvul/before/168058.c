bool AutofillManager::IsShowingUnmaskPrompt() {
  return full_card_request_ && full_card_request_->IsGettingFullCard();
}
