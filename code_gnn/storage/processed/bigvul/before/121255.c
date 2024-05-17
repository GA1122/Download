void HTMLInputElement::setSelectionRangeForBinding(int start, int end, ExceptionCode& ec)
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return;
    }
    HTMLTextFormControlElement::setSelectionRange(start, end);
}
