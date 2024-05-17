bool RenderBlock::updateLogicalWidthAndColumnWidth()
{
    LayoutUnit oldWidth = logicalWidth();
    LayoutUnit oldColumnWidth = desiredColumnWidth();

    updateLogicalWidth();
    calcColumnWidth();

    bool hasBorderOrPaddingLogicalWidthChanged = m_hasBorderOrPaddingLogicalWidthChanged;
    m_hasBorderOrPaddingLogicalWidthChanged = false;

    return oldWidth != logicalWidth() || oldColumnWidth != desiredColumnWidth() || hasBorderOrPaddingLogicalWidthChanged;
}
