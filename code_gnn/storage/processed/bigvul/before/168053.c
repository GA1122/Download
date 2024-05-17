bool AutofillManager::IsAutofillEnabled() const {
  return ::autofill::IsAutofillEnabled(client_->GetPrefs()) &&
         client_->IsAutofillSupported();
}
