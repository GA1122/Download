String HTMLInputElement::selectionDirectionForBinding(ExceptionCode& ec) const
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return String();
    }
    return HTMLTextFormControlElement::selectionDirection();
}
