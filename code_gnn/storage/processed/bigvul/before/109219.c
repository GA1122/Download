static void buildNodeHighlight(Node* node, const HighlightConfig& highlightConfig, Highlight* highlight)
{
    RenderObject* renderer = node->renderer();
    Frame* containingFrame = node->document().frame();

    if (!renderer || !containingFrame)
        return;

    highlight->setDataFromConfig(highlightConfig);

    if (renderer->node() && renderer->node()->isSVGElement() && !renderer->isSVGRoot())
        highlight->type = HighlightTypeRects;
    else if (renderer->isBox() || renderer->isRenderInline())
        highlight->type = HighlightTypeNode;
    buildNodeQuads(node, highlight->quads);
}
