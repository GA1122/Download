bool Document::queryCommandIndeterm(const String& commandName, ExceptionState& exceptionState)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "queryCommandIndeterm is only supported on HTML documents.");
        return false;
    }

    return command(this, commandName).state() == MixedTriState;
}
