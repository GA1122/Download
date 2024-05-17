String HTMLInputElement::validationMessage() const {
  if (!willValidate())
    return String();

  if (CustomError())
    return CustomValidationMessage();

  return input_type_->ValidationMessage(*input_type_view_).first;
}
