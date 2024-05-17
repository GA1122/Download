bool HTMLInputElement::IsValidValue(const String& value) const {
  if (!input_type_->CanSetStringValue()) {
    NOTREACHED();
    return false;
  }
  return !input_type_->TypeMismatchFor(value) &&
         !input_type_->StepMismatch(value) &&
         !input_type_->RangeUnderflow(value) &&
         !input_type_->RangeOverflow(value) &&
         !TooLong(value, kIgnoreDirtyFlag) &&
         !TooShort(value, kIgnoreDirtyFlag) &&
         !input_type_->PatternMismatch(value) &&
         !input_type_->ValueMissing(value);
}
