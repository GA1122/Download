void RenderBlockFlow::setLogicalLeftForChild(RenderBox* child, LayoutUnit logicalLeft, ApplyLayoutDeltaMode applyDelta)
{
    if (isHorizontalWritingMode()) {
        if (applyDelta == ApplyLayoutDelta && !RuntimeEnabledFeatures::repaintAfterLayoutEnabled())
            view()->addLayoutDelta(LayoutSize(child->x() - logicalLeft, 0));
        child->setX(logicalLeft);
    } else {
        if (applyDelta == ApplyLayoutDelta && !RuntimeEnabledFeatures::repaintAfterLayoutEnabled())
            view()->addLayoutDelta(LayoutSize(0, child->y() - logicalLeft));
        child->setY(logicalLeft);
    }
}
