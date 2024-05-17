Stream* XMLHttpRequest::responseStream()
{
    ASSERT(m_responseTypeCode == ResponseTypeStream);

    if (m_error || (m_state != LOADING && m_state != DONE))
        return 0;

    return m_responseStream.get();
}
