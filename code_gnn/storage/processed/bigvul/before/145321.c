static void appendServerMapMousePosition(StringBuilder& url, Event* event)
{
    if (!event->isMouseEvent())
        return;

    ASSERT(event->target());
    Node* target = event->target()->toNode();
    ASSERT(target);
    if (!isHTMLImageElement(*target))
        return;

    HTMLImageElement& imageElement = toHTMLImageElement(*target);
    if (!imageElement.isServerMap())
        return;

    LayoutObject* layoutObject = imageElement.layoutObject();
    if (!layoutObject || !layoutObject->isBox())
        return;

    LayoutPoint mapPoint(layoutObject->absoluteToLocal(FloatPoint(toMouseEvent(event)->absoluteLocation()), UseTransforms));

    mapPoint -= toLayoutBox(layoutObject)->contentBoxOffset();

    float scaleFactor = 1 / layoutObject->style()->effectiveZoom();
    mapPoint.scale(scaleFactor, scaleFactor);

    IntPoint clampedPoint(roundedIntPoint(mapPoint));
    clampedPoint.clampNegativeToZero();

    url.append('?');
    url.appendNumber(clampedPoint.x());
    url.append(',');
    url.appendNumber(clampedPoint.y());
}
