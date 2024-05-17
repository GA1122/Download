void InputType::ApplyStep(const Decimal& current,
                          double count,
                          AnyStepHandling any_step_handling,
                          TextFieldEventBehavior event_behavior,
                          ExceptionState& exception_state) {

  StepRange step_range(CreateStepRange(any_step_handling));
  if (!step_range.HasStep()) {
    exception_state.ThrowDOMException(
        kInvalidStateError,
        "This form element does not have an allowed value step.");
    return;
  }

  if (step_range.Minimum() > step_range.Maximum())
    return;

  Decimal aligned_maximum = step_range.StepSnappedMaximum();
  if (!aligned_maximum.IsFinite())
    return;

  Decimal base = step_range.StepBase();
  Decimal step = step_range.Step();
  EventQueueScope scope;
  Decimal new_value = current;
  const AtomicString& step_string = GetElement().FastGetAttribute(stepAttr);
  if (!DeprecatedEqualIgnoringCase(step_string, "any") &&
      step_range.StepMismatch(current)) {

    DCHECK(!step.IsZero());
    if (count < 0) {
      new_value = base + ((new_value - base) / step).Floor() * step;
      ++count;
    } else if (count > 0) {
      new_value = base + ((new_value - base) / step).Ceil() * step;
      --count;
    }
  }
  new_value = new_value + step_range.Step() * Decimal::FromDouble(count);

  if (!DeprecatedEqualIgnoringCase(step_string, "any"))
    new_value = step_range.AlignValueForStep(current, new_value);

  if (new_value < step_range.Minimum()) {
    const Decimal aligned_minimum =
        base + ((step_range.Minimum() - base) / step).Ceil() * step;
    DCHECK_GE(aligned_minimum, step_range.Minimum());
    new_value = aligned_minimum;
  }

  if (new_value > step_range.Maximum())
    new_value = aligned_maximum;

  SetValueAsDecimal(new_value, event_behavior, exception_state);

  if (AXObjectCache* cache = GetElement().GetDocument().ExistingAXObjectCache())
    cache->HandleValueChanged(&GetElement());
}
