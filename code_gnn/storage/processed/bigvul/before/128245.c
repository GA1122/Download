Color FrameView::documentBackgroundColor() const
{

    Color result = baseBackgroundColor();
    if (!frame().document())
        return result;

    Element* htmlElement = frame().document()->documentElement();
    Element* bodyElement = frame().document()->body();

    if (htmlElement && htmlElement->renderer())
        result = result.blend(htmlElement->renderer()->style()->visitedDependentColor(CSSPropertyBackgroundColor));
    if (bodyElement && bodyElement->renderer())
        result = result.blend(bodyElement->renderer()->style()->visitedDependentColor(CSSPropertyBackgroundColor));

    return result;
}
