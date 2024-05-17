Range::CompareResults Range::compareNode(Node* refNode, ExceptionCode& ec) const
{

    if (!refNode) {
        ec = NOT_FOUND_ERR;
        return NODE_BEFORE;
    }
    
    if (!m_start.container() && refNode->attached()) {
        ec = INVALID_STATE_ERR;
        return NODE_BEFORE;
    }

    if (m_start.container() && !refNode->attached()) {
        return NODE_BEFORE;
    }

    if (refNode->document() != m_ownerDocument) {
        return NODE_BEFORE;
    }

    ContainerNode* parentNode = refNode->parentNode();
    int nodeIndex = refNode->nodeIndex();
    
    if (!parentNode) {
        ec = NOT_FOUND_ERR;
        return NODE_BEFORE;
    }

    if (comparePoint(parentNode, nodeIndex, ec) < 0) {  
        if (comparePoint(parentNode, nodeIndex + 1, ec) > 0)  
            return NODE_BEFORE_AND_AFTER;
        return NODE_BEFORE;  
    } else {  
        if (comparePoint(parentNode, nodeIndex + 1, ec) > 0)  
            return NODE_AFTER;
        return NODE_INSIDE;  
    }
}
