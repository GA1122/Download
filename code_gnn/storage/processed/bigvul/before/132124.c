void LayoutBlockFlow::setLogicalLeftForChild(LayoutBox& child, LayoutUnit logicalLeft)
{
    if (isHorizontalWritingMode()) {
        child.setX(logicalLeft);
    } else {
        child.setY(logicalLeft);
    }
}
