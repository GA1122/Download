ContainerNode::~ContainerNode()
{
    if (Document* document = documentInternal())
        willBeDeletedFrom(document);
    removeDetachedChildren();
}
