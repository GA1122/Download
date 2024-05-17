void HTMLFormControlElement::dispatchFocusEvent(Element* oldFocusedElement, WebFocusType type, InputDeviceCapabilities* sourceCapabilities)
{
    if (type != WebFocusTypePage)
        m_wasFocusedByMouse = type == WebFocusTypeMouse;
    HTMLElement::dispatchFocusEvent(oldFocusedElement, type, sourceCapabilities);
}
