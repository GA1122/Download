String HTMLInputElement::GetPlaceholderValue() const {
  return !SuggestedValue().IsEmpty() ? SuggestedValue() : StrippedPlaceholder();
}
