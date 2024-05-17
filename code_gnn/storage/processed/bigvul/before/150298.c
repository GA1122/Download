void PasswordAutofillAgent::FocusStateNotifier::FocusedInputChanged(
    FocusedFieldType focused_field_type) {
  if (focused_field_type != focused_field_type_ ||
      IsFillable(focused_field_type)) {
    agent_->GetPasswordManagerDriver()->FocusedInputChanged(focused_field_type);
  }

  focused_field_type_ = focused_field_type;
}
