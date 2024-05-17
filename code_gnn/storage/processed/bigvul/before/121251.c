void HTMLInputElement::setRangeText(const String& replacement, ExceptionCode& ec)
{
    if (!m_inputType->supportsSelectionAPI()) {
        ec = INVALID_STATE_ERR;
        return;
    }

    HTMLTextFormControlElement::setRangeText(replacement, ec);
}
