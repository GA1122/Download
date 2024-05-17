ScriptString XMLHttpRequest::responseText(ExceptionState& es)
{
    if (m_responseTypeCode != ResponseTypeDefault && m_responseTypeCode != ResponseTypeText) {
        es.throwDOMException(InvalidStateError, ExceptionMessages::failedToGet("responseText", "XMLHttpRequest", "the value is only accessible if the object's 'responseType' is '' or 'text' (was '" + responseType() + "')."));
        return ScriptString();
    }
    if (m_error || (m_state != LOADING && m_state != DONE))
        return ScriptString();
    return m_responseText;
}
