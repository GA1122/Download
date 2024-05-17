bool ContainerNode::hasRestyleFlagInternal(DynamicRestyleFlags mask) const
{
    return rareData()->hasRestyleFlag(mask);
}
