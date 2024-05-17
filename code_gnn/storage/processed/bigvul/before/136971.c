void HTMLInputElement::SetValueFromRenderer(const String& value) {
  DCHECK_NE(type(), InputTypeNames::file);

  TextControlElement::SetSuggestedValue(String());

  DCHECK(value == input_type_->SanitizeUserInputValue(value) ||
         input_type_->SanitizeUserInputValue(value).IsEmpty());

  DCHECK(!value.IsNull());
  SetValueBeforeFirstUserEditIfNotSet();
  non_attribute_value_ = value;
  has_dirty_value_ = true;
  needs_to_update_view_value_ = false;
  CheckIfValueWasReverted(value);

  if (!IsTextField())
    DispatchInputEvent();
  NotifyFormStateChanged();

  SetNeedsValidityCheck();

  SetAutofilled(false);
}
