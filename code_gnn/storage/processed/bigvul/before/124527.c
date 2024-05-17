void RenderBlock::computeOverflow(LayoutUnit oldClientAfterEdge, bool)
{
    m_overflow.clear();

    addOverflowFromChildren();

    addOverflowFromPositionedObjects();

    if (hasOverflowClip()) {
        LayoutRect clientRect(noOverflowRect());
        LayoutRect rectToApply;
        if (isHorizontalWritingMode())
            rectToApply = LayoutRect(clientRect.x(), clientRect.y(), 1, max<LayoutUnit>(0, oldClientAfterEdge - clientRect.y()));
        else
            rectToApply = LayoutRect(clientRect.x(), clientRect.y(), max<LayoutUnit>(0, oldClientAfterEdge - clientRect.x()), 1);
        addLayoutOverflow(rectToApply);
        if (hasRenderOverflow())
            m_overflow->setLayoutClientAfterEdge(oldClientAfterEdge);
    }

    addVisualEffectOverflow();

    addVisualOverflowFromTheme();
}
