bool HTMLFormControlElement::shouldHaveFocusAppearance() const
{
    return !m_wasFocusedByMouse || shouldShowFocusRingOnMouseFocus();
}
