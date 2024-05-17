void ContainerNode::setRestyleFlag(DynamicRestyleFlags mask)
{
    ASSERT(isElementNode() || isShadowRoot());
    ensureRareData().setRestyleFlag(mask);
}
