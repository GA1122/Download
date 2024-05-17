void XMLHttpRequest::setRequestHeader(const AtomicString& name, const String& value, ExceptionState& es)
{
    if (m_state != OPENED || m_loader) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToExecute("setRequestHeader", "XMLHttpRequest", "the object's state must be OPENED."));
        return;
    }

    if (!isValidHTTPToken(name)) {
        es.throwDOMException(SyntaxError, ExceptionMessages::failedToExecute("setRequestHeader", "XMLHttpRequest", "'" + name + "' is not a valid HTTP header field name."));
        return;
    }

    if (!isValidHTTPHeaderValue(value)) {
        es.throwDOMException(SyntaxError, ExceptionMessages::failedToExecute("setRequestHeader", "XMLHttpRequest", "'" + value + "' is not a valid HTTP header field value."));
        return;
    }

    if (!isAllowedHTTPHeader(name)) {
        logConsoleError(scriptExecutionContext(), "Refused to set unsafe header \"" + name + "\"");
        return;
    }

    setRequestHeaderInternal(name, value);
}
