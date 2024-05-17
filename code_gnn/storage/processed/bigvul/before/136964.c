void HTMLInputElement::SetNonAttributeValue(const String& sanitized_value) {
  DCHECK_EQ(input_type_->GetValueMode(), ValueMode::kValue);
  non_attribute_value_ = sanitized_value;
  has_dirty_value_ = true;
  SetNeedsValidityCheck();
  input_type_->InRangeChanged();
}
