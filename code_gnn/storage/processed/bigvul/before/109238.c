 void InspectorOverlay::hideHighlight()
 {
     m_highlightNode.clear();
    m_eventTargetNode.clear();
    m_highlightQuad.clear();
    update();
}
