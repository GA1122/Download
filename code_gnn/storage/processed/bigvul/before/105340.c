bool AutofillManager::IsAutofillEnabled() const {
  return const_cast<AutofillManager*>(this)->tab_contents()->profile()->
      GetPrefs()->GetBoolean(prefs::kAutofillEnabled);
}
