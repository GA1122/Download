void Range::textNodeSplit(Text* oldNode)
{
    ASSERT(oldNode);
    ASSERT(oldNode->document() == m_ownerDocument);
    ASSERT(oldNode->parentNode());
    ASSERT(oldNode->isTextNode());
    ASSERT(oldNode->nextSibling());
    ASSERT(oldNode->nextSibling()->isTextNode());
    boundaryTextNodesSplit(m_start, oldNode);
    boundaryTextNodesSplit(m_end, oldNode);
}
