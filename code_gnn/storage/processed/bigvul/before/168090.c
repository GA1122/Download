bool AutofillManager::ShouldShowCreditCardSigninPromo(
    const FormData& form,
    const FormFieldData& field) {
  AutofillField* autofill_field = GetAutofillField(form, field);
  if (!autofill_field || autofill_field->Type().group() != CREDIT_CARD ||
      !client_->ShouldShowSigninPromo())
    return false;

  if (IsFormNonSecure(form))
    return false;

  int impression_count = client_->GetPrefs()->GetInteger(
      prefs::kAutofillCreditCardSigninPromoImpressionCount);
  if (impression_count < kCreditCardSigninPromoImpressionLimit) {
    client_->GetPrefs()->SetInteger(
        prefs::kAutofillCreditCardSigninPromoImpressionCount,
        impression_count + 1);
    return true;
  }

  return false;
}
