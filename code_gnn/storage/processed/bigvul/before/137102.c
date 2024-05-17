void InputType::SetValue(const String& sanitized_value,
                         bool value_changed,
                         TextFieldEventBehavior event_behavior,
                         TextControlSetValueSelection) {
  DCHECK_EQ(GetValueMode(), ValueMode::kValue);
  if (event_behavior == kDispatchNoEvent)
    GetElement().SetNonAttributeValue(sanitized_value);
  else
    GetElement().SetNonAttributeValueByUserEdit(sanitized_value);
  if (!value_changed)
    return;
  switch (event_behavior) {
    case kDispatchChangeEvent:
      GetElement().DispatchFormControlChangeEvent();
      break;
    case kDispatchInputAndChangeEvent:
      GetElement().DispatchInputEvent();
      GetElement().DispatchFormControlChangeEvent();
      break;
    case kDispatchNoEvent:
      break;
  }
}
