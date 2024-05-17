bool InputType::IsOutOfRange(const String& value) const {
  if (!IsSteppable())
    return false;

  const Decimal numeric_value = ParseToNumberOrNaN(value);
  if (!numeric_value.IsFinite())
    return false;

  StepRange step_range(CreateStepRange(kRejectAny));
  return step_range.HasRangeLimitations() &&
         (numeric_value < step_range.Minimum() ||
          numeric_value > step_range.Maximum());
}
