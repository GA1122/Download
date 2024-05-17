int HTMLInputElement::selectionEndForBinding(ExceptionCode& ec) const
{
    if (!canHaveSelection()) {
        ec = INVALID_STATE_ERR;
        return 0;
    }
    return HTMLTextFormControlElement::selectionEnd();
}
