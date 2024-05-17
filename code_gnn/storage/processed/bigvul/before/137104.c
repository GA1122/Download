void InputType::SetValueAsDecimal(const Decimal& new_value,
                                  TextFieldEventBehavior event_behavior,
                                  ExceptionState&) const {
  GetElement().setValue(Serialize(new_value), event_behavior);
}
