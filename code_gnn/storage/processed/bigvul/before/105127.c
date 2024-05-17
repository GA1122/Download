void Range::insertNode(PassRefPtr<Node> prpNewNode, ExceptionCode& ec)
{
    RefPtr<Node> newNode = prpNewNode;

    ec = 0;

    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return;
    }

    if (!newNode) {
        ec = NOT_FOUND_ERR;
        return;
    }

    if (containedByReadOnly()) {
        ec = NO_MODIFICATION_ALLOWED_ERR;
        return;
    }


    bool startIsText = m_start.container()->isTextNode();
    if (startIsText && !m_start.container()->parentNode()) {
        ec = HIERARCHY_REQUEST_ERR;
        return;
    }

    Node* checkAgainst;
    if (startIsText)
        checkAgainst = m_start.container()->parentNode();
    else
        checkAgainst = m_start.container();

    Node::NodeType newNodeType = newNode->nodeType();
    int numNewChildren;
    if (newNodeType == Node::DOCUMENT_FRAGMENT_NODE && !newNode->isShadowRoot()) {
        numNewChildren = 0;
        for (Node* c = newNode->firstChild(); c; c = c->nextSibling()) {
            if (!checkAgainst->childTypeAllowed(c->nodeType())) {
                ec = HIERARCHY_REQUEST_ERR;
                return;
            }
            ++numNewChildren;
        }
    } else {
        numNewChildren = 1;
        if (!checkAgainst->childTypeAllowed(newNodeType)) {
            ec = HIERARCHY_REQUEST_ERR;
            return;
        }
    }

    for (Node* n = m_start.container(); n; n = n->parentNode()) {
        if (n == newNode) {
            ec = HIERARCHY_REQUEST_ERR;
            return;
        }
    }

    switch (newNodeType) {
    case Node::ATTRIBUTE_NODE:
    case Node::ENTITY_NODE:
    case Node::NOTATION_NODE:
    case Node::DOCUMENT_NODE:
        ec = RangeException::INVALID_NODE_TYPE_ERR;
        return;
    default:
        if (newNode->isShadowRoot()) {
            ec = RangeException::INVALID_NODE_TYPE_ERR;
            return;
        }
        break;
    }

    bool collapsed = m_start == m_end;
    RefPtr<Node> container;
    if (startIsText) {
        container = m_start.container();
        RefPtr<Text> newText = toText(container.get())->splitText(m_start.offset(), ec);
        if (ec)
            return;
        
        container = m_start.container();
        container->parentNode()->insertBefore(newNode.release(), newText.get(), ec);
        if (ec)
            return;

        if (collapsed)
            m_end.setToBeforeChild(newText.get());
    } else {
        RefPtr<Node> lastChild;
        if (collapsed)
            lastChild = (newNodeType == Node::DOCUMENT_FRAGMENT_NODE) ? newNode->lastChild() : newNode;

        int startOffset = m_start.offset();
        container = m_start.container();
        container->insertBefore(newNode.release(), container->childNode(startOffset), ec);
        if (ec)
            return;

        if (collapsed && numNewChildren)
            m_end.set(m_start.container(), startOffset + numNewChildren, lastChild.get());
    }
}
