void AutofillExternalDelegate::DidAcceptSuggestion(const base::string16& value,
                                                   int identifier,
                                                   int position) {
  if (identifier == POPUP_ITEM_ID_AUTOFILL_OPTIONS) {
    manager_->ShowAutofillSettings(popup_type_ == PopupType::kCreditCards);
  } else if (identifier == POPUP_ITEM_ID_CLEAR_FORM) {
    driver_->RendererShouldClearFilledSection();
  } else if (identifier == POPUP_ITEM_ID_PASSWORD_ENTRY ||
             identifier == POPUP_ITEM_ID_USERNAME_ENTRY) {
    NOTREACHED();   
  } else if (identifier == POPUP_ITEM_ID_DATALIST_ENTRY) {
    driver_->RendererShouldAcceptDataListSuggestion(value);
  } else if (identifier == POPUP_ITEM_ID_AUTOCOMPLETE_ENTRY) {
    driver_->RendererShouldFillFieldWithValue(value);
    AutofillMetrics::LogAutocompleteSuggestionAcceptedIndex(position);
  } else if (identifier == POPUP_ITEM_ID_SCAN_CREDIT_CARD) {
    manager_->client()->ScanCreditCard(base::Bind(
        &AutofillExternalDelegate::OnCreditCardScanned, GetWeakPtr()));
  } else if (identifier == POPUP_ITEM_ID_CREDIT_CARD_SIGNIN_PROMO) {
    manager_->client()->ExecuteCommand(identifier);
  } else if (identifier == POPUP_ITEM_ID_SHOW_ACCOUNT_CARDS) {
    manager_->OnUserAcceptedCardsFromAccountOption();
  } else {
    if (identifier > 0)   
      AutofillMetrics::LogAutofillSuggestionAcceptedIndex(position);

    FillAutofillFormData(identifier, false);
  }

  if (should_show_scan_credit_card_) {
    AutofillMetrics::LogScanCreditCardPromptMetric(
        identifier == POPUP_ITEM_ID_SCAN_CREDIT_CARD
            ? AutofillMetrics::SCAN_CARD_ITEM_SELECTED
            : AutofillMetrics::SCAN_CARD_OTHER_ITEM_SELECTED);
  }

  if (identifier == POPUP_ITEM_ID_SHOW_ACCOUNT_CARDS) {
    should_show_cards_from_account_option_ = false;
    manager_->RefetchCardsAndUpdatePopup(query_id_, query_form_, query_field_);
  } else {
    manager_->client()->HideAutofillPopup();
  }
}
