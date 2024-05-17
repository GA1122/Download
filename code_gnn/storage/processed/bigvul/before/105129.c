bool Range::isPointInRange(Node* refNode, int offset, ExceptionCode& ec)
{
    if (!m_start.container()) {
        ec = INVALID_STATE_ERR;
        return false;
    }

    if (!refNode) {
        ec = HIERARCHY_REQUEST_ERR;
        return false;
    }

    if (!refNode->attached()) {
        return false;
    }

    if (refNode->document() != m_ownerDocument) {
        ec = WRONG_DOCUMENT_ERR;
        return false;
    }

    ec = 0;
    checkNodeWOffset(refNode, offset, ec);
    if (ec)
        return false;

    return compareBoundaryPoints(refNode, offset, m_start.container(), m_start.offset(), ec) >= 0 && !ec
        && compareBoundaryPoints(refNode, offset, m_end.container(), m_end.offset(), ec) <= 0 && !ec;
}
