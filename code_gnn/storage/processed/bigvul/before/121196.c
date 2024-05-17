bool HTMLInputElement::isTextFormControlKeyboardFocusable(KeyboardEvent* event) const
{
    return HTMLTextFormControlElement::isKeyboardFocusable(event);
}
