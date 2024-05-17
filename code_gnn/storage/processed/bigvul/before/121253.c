void HTMLInputElement::setSelectionDirectionForBinding(const String& direction, ExceptionCode& ec)
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return;
    }
    HTMLTextFormControlElement::setSelectionDirection(direction);
}
