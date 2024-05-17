size_t RenderFlexibleBox::numberOfInFlowPositionedChildren(const OrderedFlexItemList& children) const
{
    size_t count = 0;
    for (size_t i = 0; i < children.size(); ++i) {
        RenderBox* child = children[i];
        if (!child->isOutOfFlowPositioned())
            ++count;
    }
    return count;
}
