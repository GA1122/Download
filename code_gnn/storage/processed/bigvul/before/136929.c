String HTMLInputElement::LocalizeValue(const String& proposed_value) const {
  if (proposed_value.IsNull())
    return proposed_value;
  return input_type_->LocalizeValue(proposed_value);
}
