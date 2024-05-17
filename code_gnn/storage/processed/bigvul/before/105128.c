bool Range::intersectsNode(Node* refNode, ExceptionCode& ec)
{

    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return false;
    }
    if (!refNode) {
        ec = NOT_FOUND_ERR;
        return false;
    }

    if (!refNode->attached() || refNode->document() != m_ownerDocument) {
        return false;
    }

    ContainerNode* parentNode = refNode->parentNode();
    int nodeIndex = refNode->nodeIndex();
    
    if (!parentNode) {
        ec = NOT_FOUND_ERR;
        return false;
    }

    if (comparePoint(parentNode, nodeIndex, ec) < 0 &&  
        comparePoint(parentNode, nodeIndex + 1, ec) < 0) {  
        return false;
    } else if (comparePoint(parentNode, nodeIndex, ec) > 0 &&  
               comparePoint(parentNode, nodeIndex + 1, ec) > 0) {  
        return false;
    }
    
    return true;  
}
