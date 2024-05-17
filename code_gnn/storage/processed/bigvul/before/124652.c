    void update()
    {
        if (m_colIndex < 0)
            return;

        m_colRect = m_block.columnRectAt(const_cast<ColumnInfo*>(m_colInfo), m_colIndex);
        m_block.flipForWritingMode(m_colRect);
        m_currLogicalTopOffset -= (m_isHorizontal ? m_colRect.height() : m_colRect.width()) * m_direction;
    }
