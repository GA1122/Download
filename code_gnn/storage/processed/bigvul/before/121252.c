void HTMLInputElement::setRangeText(const String& replacement, unsigned start, unsigned end, const String& selectionMode, ExceptionCode& ec)
{
    if (!m_inputType->supportsSelectionAPI()) {
        ec = INVALID_STATE_ERR;
        return;
    }

    HTMLTextFormControlElement::setRangeText(replacement, start, end, selectionMode, ec);
}
