void HTMLInputElement::setSelectionEndForBinding(int end, ExceptionCode& ec)
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return;
    }
    HTMLTextFormControlElement::setSelectionEnd(end);
}
