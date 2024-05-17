void XMLHttpRequest::setTimeout(unsigned long timeout, ExceptionState& es)
{
    if (scriptExecutionContext()->isDocument() && !m_async) {
        es.throwDOMException(InvalidAccessError, ExceptionMessages::failedToSet("timeout", "XMLHttpRequest", "timeouts cannot be set for synchronous requests made from a document."));
        return;
    }
    m_timeoutMilliseconds = timeout;
}
