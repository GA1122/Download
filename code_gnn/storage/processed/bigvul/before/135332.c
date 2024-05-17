bool Document::queryCommandEnabled(const String& commandName, ExceptionState& exceptionState)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "queryCommandEnabled is only supported on HTML documents.");
        return false;
    }

    return command(this, commandName).isEnabled();
}
