void RenderBox::updateLogicalHeight()
{
    m_intrinsicContentLogicalHeight = contentLogicalHeight();

    LogicalExtentComputedValues computedValues;
    computeLogicalHeight(logicalHeight(), logicalTop(), computedValues);

    setLogicalHeight(computedValues.m_extent);
    setLogicalTop(computedValues.m_position);
    setMarginBefore(computedValues.m_margins.m_before);
    setMarginAfter(computedValues.m_margins.m_after);
}
