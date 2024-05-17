inline StyledMarkupAccumulator::StyledMarkupAccumulator(Vector<Node*>* nodes, EAbsoluteURLs shouldResolveURLs, EAnnotateForInterchange shouldAnnotate,
    const Range* range, Node* highestNodeToBeSerialized)
    : MarkupAccumulator(nodes, shouldResolveURLs, range)
    , m_shouldAnnotate(shouldAnnotate)
    , m_highestNodeToBeSerialized(highestNodeToBeSerialized)
{
}
