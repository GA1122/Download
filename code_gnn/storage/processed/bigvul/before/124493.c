    void advance()
    {
        ASSERT(hasMore());
        m_colIndex--;
        update();
    }
