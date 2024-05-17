String XMLHttpRequest::statusText(ExceptionState& es) const
{
    if (!m_response.httpStatusText().isNull())
        return m_response.httpStatusText();

    if (m_state == OPENED) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToGet("statusText", "XMLHttpRequest", "the object's state must not be OPENED."));
    }

    return String();
}
