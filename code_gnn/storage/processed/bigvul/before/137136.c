void InputType::WarnIfValueIsInvalidAndElementIsVisible(
    const String& value) const {
  const ComputedStyle* style = GetElement().GetComputedStyle();
  if (style && style->Visibility() != EVisibility::kHidden)
    WarnIfValueIsInvalid(value);
}
