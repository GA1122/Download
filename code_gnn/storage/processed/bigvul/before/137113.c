bool InputType::StepMismatch(const String& value) const {
  if (!IsSteppable())
    return false;

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return false;

  return CreateStepRange(kRejectAny).StepMismatch(numeric_value);
}
