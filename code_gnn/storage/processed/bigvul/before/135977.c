ContainerNode::~ContainerNode()
{
    ASSERT(needsAttach());
#if !ENABLE(OILPAN)
    willBeDeletedFromDocument();
    removeDetachedChildren();
#endif
}
