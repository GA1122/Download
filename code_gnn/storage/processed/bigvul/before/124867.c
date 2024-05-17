RenderBox* RenderBox::findAutoscrollable(RenderObject* renderer)
{
    while (renderer && !(renderer->isBox() && toRenderBox(renderer)->canAutoscroll())) {
        if (!renderer->parent() && renderer->node() == renderer->document() && renderer->document().ownerElement())
            renderer = renderer->document().ownerElement()->renderer();
        else
            renderer = renderer->parent();
    }

    return renderer && renderer->isBox() ? toRenderBox(renderer) : 0;
}
