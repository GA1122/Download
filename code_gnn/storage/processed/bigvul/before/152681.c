void HTMLFormControlElement::willCallDefaultEventHandler(const Event& event) {
  if (!m_wasFocusedByMouse)
    return;
  if (!event.isKeyboardEvent() || event.type() != EventTypeNames::keydown)
    return;

  bool oldShouldHaveFocusAppearance = shouldHaveFocusAppearance();
  m_wasFocusedByMouse = false;

  if (oldShouldHaveFocusAppearance != shouldHaveFocusAppearance() &&
      layoutObject())
    LayoutTheme::theme().controlStateChanged(*layoutObject(),
                                             FocusControlState);
}
