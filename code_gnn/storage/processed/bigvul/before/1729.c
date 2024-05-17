    Burr(IRCView* o, Burr* prev, QTextBlock b, int objFormat)
        : m_block(b), m_format(objFormat), m_prev(prev), m_next(0),
        m_owner(o)
    {
        if (m_prev)
            m_prev->m_next = this;
    }
