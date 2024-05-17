void ContainerNode::insertBeforeCommon(Node* nextChild, Node* newChild)
{
    NoEventDispatchAssertion assertNoEventDispatch;

    ASSERT(newChild);
    ASSERT(!newChild->parentNode());  
    ASSERT(!newChild->nextSibling());
    ASSERT(!newChild->previousSibling());
    ASSERT(!newChild->isShadowRoot());

    Node* prev = nextChild->previousSibling();
    ASSERT(m_lastChild != prev);
    nextChild->setPreviousSibling(newChild);
    if (prev) {
        ASSERT(m_firstChild != nextChild);
        ASSERT(prev->nextSibling() == nextChild);
        prev->setNextSibling(newChild);
    } else {
        ASSERT(m_firstChild == nextChild);
        m_firstChild = newChild;
    }
    newChild->setParentOrShadowHostNode(this);
    newChild->setPreviousSibling(prev);
    newChild->setNextSibling(nextChild);
}
