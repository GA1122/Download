void Range::getBorderAndTextQuads(Vector<FloatQuad>& quads) const
{
    Node* startContainer = m_start.container();
    Node* endContainer = m_end.container();
    Node* stopNode = pastLastNode();

    HashSet<Node*> nodeSet;
    for (Node* node = firstNode(); node != stopNode; node = node->traverseNextNode()) {
        if (node->isElementNode())
            nodeSet.add(node);
    }

    for (Node* node = firstNode(); node != stopNode; node = node->traverseNextNode()) {
        if (node->isElementNode()) {
            if (!nodeSet.contains(node->parentNode())) {
                if (RenderBoxModelObject* renderBoxModelObject = static_cast<Element*>(node)->renderBoxModelObject()) {
                    Vector<FloatQuad> elementQuads;
                    renderBoxModelObject->absoluteQuads(elementQuads);
                    adjustFloatQuadsForScrollAndAbsoluteZoomAndPageScale(elementQuads, m_ownerDocument.get(), renderBoxModelObject);

                    quads.append(elementQuads);
                }
            }
        } else if (node->isTextNode()) {
            if (RenderObject* renderer = toText(node)->renderer()) {
                RenderText* renderText = toRenderText(renderer);
                int startOffset = (node == startContainer) ? m_start.offset() : 0;
                int endOffset = (node == endContainer) ? m_end.offset() : INT_MAX;
                
                Vector<FloatQuad> textQuads;
                renderText->absoluteQuadsForRange(textQuads, startOffset, endOffset);
                adjustFloatQuadsForScrollAndAbsoluteZoomAndPageScale(textQuads, m_ownerDocument.get(), renderText);

                quads.append(textQuads);
            }
        }
    }
}
