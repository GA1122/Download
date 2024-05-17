bool RenderFlexibleBox::childPreferredMainAxisContentExtentRequiresLayout(RenderBox* child, bool hasInfiniteLineLength) const
{
    return preferredMainAxisExtentDependsOnLayout(flexBasisForChild(child), hasInfiniteLineLength) && hasOrthogonalFlow(child);
}
