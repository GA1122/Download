ScriptString XMLHttpRequest::responseJSONSource()
{
    ASSERT(m_responseTypeCode == ResponseTypeJSON);

    if (m_error || m_state != DONE)
        return ScriptString();
    return m_responseText;
}
