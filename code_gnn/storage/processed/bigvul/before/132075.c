void LayoutBlockFlow::invalidatePaintForOverflow()
{
    LayoutUnit paintInvalidationLogicalLeft = logicalLeftVisualOverflow();
    LayoutUnit paintInvalidationLogicalRight = logicalRightVisualOverflow();
    if (hasOverflowClip()) {
        paintInvalidationLogicalLeft = std::min(paintInvalidationLogicalLeft, logicalLeftLayoutOverflow());
        paintInvalidationLogicalRight = std::max(paintInvalidationLogicalRight, logicalRightLayoutOverflow());
    }

    LayoutRect paintInvalidationRect;
    if (isHorizontalWritingMode())
        paintInvalidationRect = LayoutRect(paintInvalidationLogicalLeft, m_paintInvalidationLogicalTop, paintInvalidationLogicalRight - paintInvalidationLogicalLeft, m_paintInvalidationLogicalBottom - m_paintInvalidationLogicalTop);
    else
        paintInvalidationRect = LayoutRect(m_paintInvalidationLogicalTop, paintInvalidationLogicalLeft, m_paintInvalidationLogicalBottom - m_paintInvalidationLogicalTop, paintInvalidationLogicalRight - paintInvalidationLogicalLeft);

    if (hasOverflowClip()) {
        paintInvalidationRect.move(-scrolledContentOffset());

        paintInvalidationRect.intersect(LayoutRect(LayoutPoint(), size()));
    }

    if (!paintInvalidationRect.isEmpty()) {
        DisableCompositingQueryAsserts disabler;

        invalidatePaintRectangle(paintInvalidationRect);  
        if (hasReflection())
            invalidatePaintRectangle(reflectedRect(paintInvalidationRect));
    }

    m_paintInvalidationLogicalTop = 0;
    m_paintInvalidationLogicalBottom = 0;
}
