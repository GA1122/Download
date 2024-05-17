void Document::getCSSCanvasContext(const String& type, const String& name, int width, int height, CanvasRenderingContext2DOrWebGLRenderingContext& returnValue)
{
    HTMLCanvasElement& element = getCSSCanvasElement(name);
    element.setSize(IntSize(width, height));
    element.getContext(type, CanvasContextCreationAttributes(), returnValue);
}
