void AutofillManager::OnFocusOnFormFieldImpl(const FormData& form,
                                             const FormFieldData& field,
                                             const gfx::RectF& bounding_box) {
  if (!external_delegate_->HasActiveScreenReader())
    return;

  std::vector<Suggestion> suggestions;
  SuggestionsContext context;
  GetAvailableSuggestions(form, field, &suggestions, &context);

  external_delegate_->OnAutofillAvailabilityEvent(
      context.suppress_reason == SuppressReason::kNotSuppressed &&
      !suggestions.empty());
}
