bool AutofillManager::IsCreditCardAutofillEnabled() {
  return client_->GetPrefs()->GetBoolean(prefs::kAutofillCreditCardEnabled) &&
         client_->IsAutofillSupported();
}
