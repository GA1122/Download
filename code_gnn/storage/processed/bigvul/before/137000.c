String HTMLInputElement::ValidationSubMessage() const {
  if (!willValidate() || CustomError())
    return String();
  return input_type_->ValidationMessage(*input_type_view_).second;
}
