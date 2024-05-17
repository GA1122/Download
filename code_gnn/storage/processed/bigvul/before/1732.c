        ScrollBarPin(QScrollBar *scrollBar) : m_bar(scrollBar)
        {
            if (m_bar)
                m_bar = m_bar->value() == m_bar->maximum()? m_bar : 0;
        }
