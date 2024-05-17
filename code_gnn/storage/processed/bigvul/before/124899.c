LayoutUnit RenderBox::maxPreferredLogicalWidth() const
{
    if (preferredLogicalWidthsDirty()) {
#ifndef NDEBUG
        SetLayoutNeededForbiddenScope layoutForbiddenScope(const_cast<RenderBox&>(*this));
#endif
        const_cast<RenderBox*>(this)->computePreferredLogicalWidths();
    }

    return m_maxPreferredLogicalWidth;
}