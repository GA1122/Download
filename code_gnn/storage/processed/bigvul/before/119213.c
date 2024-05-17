int XMLHttpRequest::status(ExceptionState& es) const
{
    if (m_response.httpStatusCode())
        return m_response.httpStatusCode();

    if (m_state == OPENED) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToGet("status", "XMLHttpRequest", "the object's state must not be OPENED."));
    }

    return 0;
}
