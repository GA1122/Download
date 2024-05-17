CanvasRenderingContext* Document::getCSSCanvasContext(const String& type, const String& name, int width, int height)
{
    HTMLCanvasElement* element = getCSSCanvasElement(name);
    if (!element)
        return 0;
    element->setSize(IntSize(width, height));
    return element->getContext(type);
}
