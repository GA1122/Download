bool Document::queryCommandState(const String& commandName, ExceptionState& exceptionState)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "queryCommandState is only supported on HTML documents.");
        return false;
    }

    return command(this, commandName).state() == TrueTriState;
}
