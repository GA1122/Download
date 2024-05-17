void RenderBlockFlow::repaintOverflow()
{
    LayoutUnit repaintLogicalLeft = logicalLeftVisualOverflow();
    LayoutUnit repaintLogicalRight = logicalRightVisualOverflow();
    if (hasOverflowClip()) {
        repaintLogicalLeft = min(repaintLogicalLeft, logicalLeftLayoutOverflow());
        repaintLogicalRight = max(repaintLogicalRight, logicalRightLayoutOverflow());
    }

    LayoutRect repaintRect;
    if (isHorizontalWritingMode())
        repaintRect = LayoutRect(repaintLogicalLeft, m_repaintLogicalTop, repaintLogicalRight - repaintLogicalLeft, m_repaintLogicalBottom - m_repaintLogicalTop);
    else
        repaintRect = LayoutRect(m_repaintLogicalTop, repaintLogicalLeft, m_repaintLogicalBottom - m_repaintLogicalTop, repaintLogicalRight - repaintLogicalLeft);

    adjustRectForColumns(repaintRect);

    if (hasOverflowClip()) {
        repaintRect.move(-scrolledContentOffset());

        repaintRect.intersect(LayoutRect(LayoutPoint(), size()));
    }

    if (!repaintRect.isEmpty()) {
        DisableCompositingQueryAsserts disabler;

        repaintRectangle(repaintRect);  
        if (hasReflection())
            repaintRectangle(reflectedRect(repaintRect));
    }

    m_repaintLogicalTop = 0;
    m_repaintLogicalBottom = 0;
}
