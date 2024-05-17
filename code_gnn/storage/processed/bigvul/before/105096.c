void Range::checkDeleteExtract(ExceptionCode& ec)
{
    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return;
    }

    ec = 0;
    if (!commonAncestorContainer(ec) || ec)
        return;
        
    Node* pastLast = pastLastNode();
    for (Node* n = firstNode(); n != pastLast; n = n->traverseNextNode()) {
        if (n->isReadOnlyNode()) {
            ec = NO_MODIFICATION_ALLOWED_ERR;
            return;
        }
        if (n->nodeType() == Node::DOCUMENT_TYPE_NODE) {
            ec = HIERARCHY_REQUEST_ERR;
            return;
        }
    }

    if (containedByReadOnly()) {
        ec = NO_MODIFICATION_ALLOWED_ERR;
        return;
    }
}
