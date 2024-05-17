bool XMLHttpRequest::initSend(ExceptionState& es)
{
    if (!scriptExecutionContext())
        return false;

    if (m_state != OPENED || m_loader) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToExecute("send", "XMLHttpRequest", "the object's state must be OPENED."));
        return false;
    }

    m_error = false;
    return true;
}
