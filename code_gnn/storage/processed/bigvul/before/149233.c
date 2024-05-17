bool HTMLFormControlElement::IsKeyboardFocusable() const {
  if (RuntimeEnabledFeatures::FocuslessSpatialNavigationEnabled())
    return HTMLElement::IsKeyboardFocusable();

  return IsFocusable();
}
