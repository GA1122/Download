bool AutofillExternalDelegate::RemoveSuggestion(const base::string16& value,
                                                int identifier) {
  if (identifier > 0)
    return manager_->RemoveAutofillProfileOrCreditCard(identifier);

  if (identifier == POPUP_ITEM_ID_AUTOCOMPLETE_ENTRY) {
    manager_->RemoveAutocompleteEntry(query_field_.name, value);
    return true;
  }

  return false;
}
