void Range::detach(ExceptionCode& ec)
{
    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return;
    }

    m_ownerDocument->detachRange(this);

    m_start.clear();
    m_end.clear();
}
