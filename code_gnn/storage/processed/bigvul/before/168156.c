void VerifySubmitFormUkm(const ukm::TestAutoSetUkmRecorder& ukm_recorder,
                         const FormData& form,
                         AutofillMetrics::AutofillFormSubmittedState state,
                         bool is_for_credit_card,
                         const std::set<FormType>& form_types) {
  VerifyFormInteractionUkm(
      ukm_recorder, form, UkmFormSubmittedType::kEntryName,
      {{{UkmFormSubmittedType::kAutofillFormSubmittedStateName, state},
        {UkmSuggestionFilledType::kMillisecondsSinceFormParsedName, 0},
        {UkmFormSubmittedType::kIsForCreditCardName, is_for_credit_card},
        {UkmFormSubmittedType::kFormTypesName,
         AutofillMetrics::FormTypesToBitVector(form_types)},
        {UkmFormSubmittedType::kFormSignatureName,
         Collapse(CalculateFormSignature(form))}}});
}
