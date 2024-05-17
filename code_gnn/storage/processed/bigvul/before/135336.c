String Document::queryCommandValue(const String& commandName, ExceptionState& exceptionState)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "queryCommandValue is only supported on HTML documents.");
        return "";
    }

    return command(this, commandName).value();
}
