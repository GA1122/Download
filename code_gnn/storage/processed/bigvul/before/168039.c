void AutofillManager::FillOrPreviewForm(
    AutofillDriver::RendererFormDataAction action,
    int query_id,
    const FormData& form,
    const FormFieldData& field,
    int unique_id) {
  if (!IsValidFormData(form) || !IsValidFormFieldData(field))
    return;

  if (!RefreshDataModels() || !driver()->RendererIsAvailable())
    return;

  const CreditCard* credit_card = nullptr;
  const AutofillProfile* profile = nullptr;
  if (GetCreditCard(unique_id, &credit_card))
    FillOrPreviewCreditCardForm(action, query_id, form, field, *credit_card);
  else if (GetProfile(unique_id, &profile))
    FillOrPreviewProfileForm(action, query_id, form, field, *profile);
}
