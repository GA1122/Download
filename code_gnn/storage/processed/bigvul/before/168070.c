void AutofillManager::OnFullCardRequestSucceeded(
    const payments::FullCardRequest& full_card_request,
    const CreditCard& card,
    const base::string16& cvc) {
  FormStructure* form_structure = nullptr;
  AutofillField* autofill_field = nullptr;
  if (!GetCachedFormAndField(unmasking_form_, unmasking_field_, &form_structure,
                             &autofill_field))
    return;
  credit_card_form_event_logger_->OnDidFillSuggestion(
      masked_card_, *form_structure, *autofill_field);
  FillCreditCardForm(unmasking_query_id_, unmasking_form_, unmasking_field_,
                     card, cvc);
  masked_card_ = CreditCard();
}
