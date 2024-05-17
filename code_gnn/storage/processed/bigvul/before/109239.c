void InspectorOverlay::highlightNode(Node* node, Node* eventTarget, const HighlightConfig& highlightConfig)
{
    m_nodeHighlightConfig = highlightConfig;
    m_highlightNode = node;
    m_eventTargetNode = eventTarget;
    update();
}
