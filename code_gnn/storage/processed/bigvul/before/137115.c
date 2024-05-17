void InputType::StepUpFromLayoutObject(int n) {

  DCHECK(IsSteppable());
  if (!IsSteppable())
    return;
  DCHECK(n);
  if (!n)
    return;

  StepRange step_range(CreateStepRange(kAnyIsDefaultStep));

  if (!step_range.HasStep())
    return;

  EventQueueScope scope;
  const Decimal step = step_range.Step();

  int sign;
  if (step > 0)
    sign = n;
  else if (step < 0)
    sign = -n;
  else
    sign = 0;

  Decimal current = ParseToNumberOrNaN(GetElement().value());
  if (!current.IsFinite()) {
    current = DefaultValueForStepUp();
    const Decimal next_diff = step * n;
    if (current < step_range.Minimum() - next_diff)
      current = step_range.Minimum() - next_diff;
    if (current > step_range.Maximum() - next_diff)
      current = step_range.Maximum() - next_diff;
    SetValueAsDecimal(current, kDispatchNoEvent, IGNORE_EXCEPTION_FOR_TESTING);
  }
  if ((sign > 0 && current < step_range.Minimum()) ||
      (sign < 0 && current > step_range.Maximum())) {
    SetValueAsDecimal(sign > 0 ? step_range.Minimum() : step_range.Maximum(),
                      kDispatchChangeEvent, IGNORE_EXCEPTION_FOR_TESTING);
    return;
  }
  if ((sign > 0 && current >= step_range.Maximum()) ||
      (sign < 0 && current <= step_range.Minimum()))
    return;
  ApplyStep(current, n, kAnyIsDefaultStep, kDispatchChangeEvent,
            IGNORE_EXCEPTION_FOR_TESTING);
}
