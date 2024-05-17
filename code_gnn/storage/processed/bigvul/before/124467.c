    ColumnRectIterator(const RenderBlock& block)
        : m_block(block)
        , m_colInfo(block.columnInfo())
        , m_direction(m_block.style()->isFlippedBlocksWritingMode() ? 1 : -1)
        , m_isHorizontal(block.isHorizontalWritingMode())
        , m_logicalLeft(block.logicalLeftOffsetForContent())
    {
        int colCount = m_colInfo->columnCount();
        m_colIndex = colCount - 1;
        m_currLogicalTopOffset = colCount * m_colInfo->columnHeight() * m_direction;
        update();
    }
