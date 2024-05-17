void LayoutBlockFlow::markDescendantsWithFloatsForLayoutIfNeeded(LayoutBlockFlow& child, LayoutUnit newLogicalTop, LayoutUnit previousFloatLogicalBottom)
{
    bool markDescendantsWithFloats = false;
    if (newLogicalTop != child.logicalTop() && !child.avoidsFloats() && child.containsFloats()) {
        markDescendantsWithFloats = true;
    } else if (UNLIKELY(newLogicalTop.mightBeSaturated())) {
        markDescendantsWithFloats = true;
    } else if (!child.avoidsFloats() || child.shrinkToAvoidFloats()) {
        if (std::max(previousFloatLogicalBottom, lowestFloatLogicalBottom()) > newLogicalTop)
            markDescendantsWithFloats = true;
    }

    if (markDescendantsWithFloats)
        child.markAllDescendantsWithFloatsForLayout();
}
