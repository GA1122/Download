void HTMLInputElement::SetSuggestedValue(const String& value) {
  if (!input_type_->CanSetSuggestedValue())
    return;
  needs_to_update_view_value_ = true;
  TextControlElement::SetSuggestedValue(SanitizeValue(value));
  SetNeedsStyleRecalc(
      kSubtreeStyleChange,
      StyleChangeReasonForTracing::Create(StyleChangeReason::kControlValue));
  input_type_view_->UpdateView();
}
