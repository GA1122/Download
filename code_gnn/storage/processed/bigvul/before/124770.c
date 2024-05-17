void RenderBlockFlow::setLogicalTopForChild(RenderBox* child, LayoutUnit logicalTop, ApplyLayoutDeltaMode applyDelta)
{
    if (isHorizontalWritingMode()) {
        if (applyDelta == ApplyLayoutDelta && !RuntimeEnabledFeatures::repaintAfterLayoutEnabled())
            view()->addLayoutDelta(LayoutSize(0, child->y() - logicalTop));
        child->setY(logicalTop);
    } else {
        if (applyDelta == ApplyLayoutDelta && !RuntimeEnabledFeatures::repaintAfterLayoutEnabled())
            view()->addLayoutDelta(LayoutSize(child->x() - logicalTop, 0));
        child->setX(logicalTop);
    }
}
