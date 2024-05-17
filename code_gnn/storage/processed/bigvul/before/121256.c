void HTMLInputElement::setSelectionRangeForBinding(int start, int end, const String& direction, ExceptionCode& ec)
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return;
    }
    HTMLTextFormControlElement::setSelectionRange(start, end, direction);
}
