static Widget* widgetForElement(Element* focusedElement)
{
    if (!focusedElement)
        return 0;
    RenderObject* renderer = focusedElement->renderer();
    if (!renderer || !renderer->isWidget())
        return 0;
    return toRenderWidget(renderer)->widget();
}
