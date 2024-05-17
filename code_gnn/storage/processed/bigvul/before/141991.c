void AutofillExternalDelegate::PossiblyRemoveAutofillWarnings(
    std::vector<Suggestion>* suggestions) {
  while (suggestions->size() > 1 &&
         IsAutofillWarningEntry(suggestions->front().frontend_id) &&
         !IsAutofillWarningEntry(suggestions->back().frontend_id)) {
    suggestions->erase(suggestions->begin());
  }
}
