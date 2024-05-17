void RenderBlock::removeLeftoverAnonymousBlock(RenderBlock* child)
{
    ASSERT(child->isAnonymousBlock());
    ASSERT(!child->childrenInline());

    if (child->continuation() || (child->firstChild() && (child->isAnonymousColumnSpanBlock() || child->isAnonymousColumnsBlock())))
        return;

    RenderObject* firstAnChild = child->m_children.firstChild();
    RenderObject* lastAnChild = child->m_children.lastChild();
    if (firstAnChild) {
        RenderObject* o = firstAnChild;
        while (o) {
            o->setParent(this);
            o = o->nextSibling();
        }
        firstAnChild->setPreviousSibling(child->previousSibling());
        lastAnChild->setNextSibling(child->nextSibling());
        if (child->previousSibling())
            child->previousSibling()->setNextSibling(firstAnChild);
        if (child->nextSibling())
            child->nextSibling()->setPreviousSibling(lastAnChild);

        if (child == m_children.firstChild())
            m_children.setFirstChild(firstAnChild);
        if (child == m_children.lastChild())
            m_children.setLastChild(lastAnChild);
    } else {
        if (child == m_children.firstChild())
            m_children.setFirstChild(child->nextSibling());
        if (child == m_children.lastChild())
            m_children.setLastChild(child->previousSibling());

        if (child->previousSibling())
            child->previousSibling()->setNextSibling(child->nextSibling());
        if (child->nextSibling())
            child->nextSibling()->setPreviousSibling(child->previousSibling());
    }

    child->children()->setFirstChild(0);
    child->m_next = 0;

    child->removeFromRenderFlowThread();

    child->setParent(0);
    child->setPreviousSibling(0);
    child->setNextSibling(0);

    child->destroy();
}
