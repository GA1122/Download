    void adjust(LayoutSize& offset) const
    {
        LayoutUnit currLogicalLeftOffset = (m_isHorizontal ? m_colRect.x() : m_colRect.y()) - m_logicalLeft;
        offset += m_isHorizontal ? LayoutSize(currLogicalLeftOffset, m_currLogicalTopOffset) : LayoutSize(m_currLogicalTopOffset, currLogicalLeftOffset);
        if (m_colInfo->progressionAxis() == ColumnInfo::BlockAxis) {
            if (m_isHorizontal)
                offset.expand(0, m_colRect.y() - m_block.borderTop() - m_block.paddingTop());
            else
                offset.expand(m_colRect.x() - m_block.borderLeft() - m_block.paddingLeft(), 0);
        }
    }
