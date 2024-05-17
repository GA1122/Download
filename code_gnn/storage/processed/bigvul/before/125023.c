bool RenderFlexibleBox::hasOrthogonalFlow(RenderBox* child) const
{
    return isHorizontalFlow() != child->isHorizontalWritingMode();
}
