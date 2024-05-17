short Range::comparePoint(Node* refNode, int offset, ExceptionCode& ec) const
{

    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return 0;
    }

    if (!refNode) {
        ec = HIERARCHY_REQUEST_ERR;
        return 0;
    }

    if (!refNode->attached() || refNode->document() != m_ownerDocument) {
        ec = WRONG_DOCUMENT_ERR;
        return 0;
    }

    ec = 0;
    checkNodeWOffset(refNode, offset, ec);
    if (ec)
        return 0;

    if (compareBoundaryPoints(refNode, offset, m_start.container(), m_start.offset(), ec) < 0)
        return -1;

    if (ec)
        return 0;

    if (compareBoundaryPoints(refNode, offset, m_end.container(), m_end.offset(), ec) > 0 && !ec)
        return 1;

    return 0;
}
