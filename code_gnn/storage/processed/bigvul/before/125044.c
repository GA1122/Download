LayoutUnit RenderFlexibleBox::preferredMainAxisContentExtentForChild(RenderBox* child, bool hasInfiniteLineLength, bool relayoutChildren)
{
    child->clearOverrideSize();

    Length flexBasis = flexBasisForChild(child);
    if (preferredMainAxisExtentDependsOnLayout(flexBasis, hasInfiniteLineLength)) {
        LayoutUnit mainAxisExtent;
        if (hasOrthogonalFlow(child)) {
            if (child->needsLayout() || relayoutChildren) {
                m_intrinsicSizeAlongMainAxis.remove(child);
                child->forceChildLayout();
                m_intrinsicSizeAlongMainAxis.set(child, child->logicalHeight());
            }
            ASSERT(m_intrinsicSizeAlongMainAxis.contains(child));
            mainAxisExtent = m_intrinsicSizeAlongMainAxis.get(child);
        } else {
            mainAxisExtent = child->maxPreferredLogicalWidth();
        }
        ASSERT(mainAxisExtent - mainAxisBorderAndPaddingExtentForChild(child) >= 0);
        return mainAxisExtent - mainAxisBorderAndPaddingExtentForChild(child);
    }
    return std::max(LayoutUnit(0), computeMainAxisExtentForChild(child, MainOrPreferredSize, flexBasis));
}
