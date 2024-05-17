PassRefPtrWillBeRawPtr<ProcessingInstruction> Document::createProcessingInstruction(const String& target, const String& data, ExceptionState& exceptionState)
{
    if (!isValidName(target)) {
        exceptionState.throwDOMException(InvalidCharacterError, "The target provided ('" + target + "') is not a valid name.");
        return nullptr;
    }
    if (data.contains("?>")) {
        exceptionState.throwDOMException(InvalidCharacterError, "The data provided ('" + data + "') contains '?>'.");
        return nullptr;
    }
    return ProcessingInstruction::create(*this, target, data);
}
