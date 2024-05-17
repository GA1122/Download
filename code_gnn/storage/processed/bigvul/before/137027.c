void HTMLInputElement::setValue(const String& value,
                                TextFieldEventBehavior event_behavior,
                                TextControlSetValueSelection selection) {
  input_type_->WarnIfValueIsInvalidAndElementIsVisible(value);
  if (!input_type_->CanSetValue(value))
    return;

  TextControlElement::SetSuggestedValue(String());

  EventQueueScope scope;
  String sanitized_value = SanitizeValue(value);
  bool value_changed = sanitized_value != this->value();

  SetLastChangeWasNotUserEdit();
  needs_to_update_view_value_ = true;

  input_type_->SetValue(sanitized_value, value_changed, event_behavior,
                        selection);
  input_type_view_->DidSetValue(sanitized_value, value_changed);

  if (value_changed)
    NotifyFormStateChanged();
}
