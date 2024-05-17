bool Range::collapsed(ExceptionCode& ec) const
{
    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return 0;
    }

    return m_start == m_end;
}