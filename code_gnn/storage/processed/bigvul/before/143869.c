PassRefPtrWillBeRawPtr<Node> NodeIterator::nextNode(ExceptionState& exceptionState)
{
    RefPtrWillBeRawPtr<Node> result = nullptr;

    m_candidateNode = m_referenceNode;
    while (m_candidateNode.moveToNext(root())) {
        RefPtrWillBeRawPtr<Node> provisionalResult = m_candidateNode.node;
        bool nodeWasAccepted = acceptNode(provisionalResult.get(), exceptionState) == NodeFilter::FILTER_ACCEPT;
        if (exceptionState.hadException())
            break;
        if (nodeWasAccepted) {
            m_referenceNode = m_candidateNode;
            result = provisionalResult.release();
            break;
        }
    }

    m_candidateNode.clear();
    return result.release();
}
