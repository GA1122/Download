        ~ScrollBarPin()
        {
            if (m_bar)
                m_bar->setValue(m_bar->maximum());
        }
