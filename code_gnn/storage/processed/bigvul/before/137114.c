void InputType::StepUp(double n, ExceptionState& exception_state) {
  if (!IsSteppable()) {
    exception_state.ThrowDOMException(kInvalidStateError,
                                      "This form element is not steppable.");
    return;
  }
  const Decimal current = ParseToNumber(GetElement().value(), 0);
  ApplyStep(current, n, kRejectAny, kDispatchNoEvent, exception_state);
}
