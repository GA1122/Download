void IRCView::blockDeleted(Burr* b)  
{
    Q_ASSERT(b);  

    if (b == m_lastMarkerLine)
        m_lastMarkerLine = b->m_prev;

    if (b == m_rememberLine)
        m_rememberLine = 0;
}
