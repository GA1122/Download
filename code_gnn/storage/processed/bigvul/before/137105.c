void InputType::SetValueAsDouble(double double_value,
                                 TextFieldEventBehavior event_behavior,
                                 ExceptionState& exception_state) const {
  exception_state.ThrowDOMException(
      kInvalidStateError, "This input element does not support Number values.");
}
