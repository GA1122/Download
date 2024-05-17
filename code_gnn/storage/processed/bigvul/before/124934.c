LayoutRect RenderBox::reflectedRect(const LayoutRect& r) const
{
    if (!style()->boxReflect())
        return LayoutRect();

    LayoutRect box = borderBoxRect();
    LayoutRect result = r;
    switch (style()->boxReflect()->direction()) {
        case ReflectionBelow:
            result.setY(box.maxY() + reflectionOffset() + (box.maxY() - r.maxY()));
            break;
        case ReflectionAbove:
            result.setY(box.y() - reflectionOffset() - box.height() + (box.maxY() - r.maxY()));
            break;
        case ReflectionLeft:
            result.setX(box.x() - reflectionOffset() - box.width() + (box.maxX() - r.maxX()));
            break;
        case ReflectionRight:
            result.setX(box.maxX() + reflectionOffset() + (box.maxX() - r.maxX()));
            break;
    }
    return result;
}
