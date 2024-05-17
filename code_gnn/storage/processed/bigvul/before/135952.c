void ContainerNode::invalidateNodeListCachesInAncestors(const QualifiedName* attrName, Element* attributeOwnerElement)
{
    if (hasRareData() && (!attrName || isAttributeNode())) {
        if (NodeListsNodeData* lists = rareData()->nodeLists()) {
            if (ChildNodeList* childNodeList = lists->childNodeList(*this))
                childNodeList->invalidateCache();
        }
    }

    if (attrName && !attributeOwnerElement)
        return;

    if (!document().shouldInvalidateNodeListCaches(attrName))
        return;

    document().invalidateNodeListCaches(attrName);

    for (ContainerNode* node = this; node; node = node->parentNode()) {
        if (NodeListsNodeData* lists = node->nodeLists())
            lists->invalidateCaches(attrName);
    }
}
