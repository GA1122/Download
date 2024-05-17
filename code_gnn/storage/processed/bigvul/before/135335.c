bool Document::queryCommandSupported(const String& commandName, ExceptionState& exceptionState)
{
    if (!isHTMLDocument() && !isXHTMLDocument()) {
        exceptionState.throwDOMException(InvalidStateError, "queryCommandSupported is only supported on HTML documents.");
        return false;
    }

    return command(this, commandName).isSupported();
}
