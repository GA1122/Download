String HTMLInputElement::ValueOrDefaultLabel() const {
  String value = this->value();
  if (!value.IsNull())
    return value;
  return input_type_->DefaultLabel();
}
