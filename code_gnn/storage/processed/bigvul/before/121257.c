void HTMLInputElement::setSelectionStartForBinding(int start, ExceptionCode& ec)
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return;
    }
    HTMLTextFormControlElement::setSelectionStart(start);
}
