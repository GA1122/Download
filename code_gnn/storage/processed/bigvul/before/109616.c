PassRefPtr<ProcessingInstruction> Document::createProcessingInstruction(const String& target, const String& data, ExceptionState& es)
{
    if (!isValidName(target)) {
        es.throwUninformativeAndGenericDOMException(InvalidCharacterError);
        return 0;
    }
    if (isHTMLDocument()) {
        es.throwUninformativeAndGenericDOMException(NotSupportedError);
        return 0;
    }
    return ProcessingInstruction::create(*this, target, data);
}
