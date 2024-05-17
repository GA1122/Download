int RenderBox::reflectionOffset() const
{
    if (!style()->boxReflect())
        return 0;
    if (style()->boxReflect()->direction() == ReflectionLeft || style()->boxReflect()->direction() == ReflectionRight)
        return valueForLength(style()->boxReflect()->offset(), borderBoxRect().width());
    return valueForLength(style()->boxReflect()->offset(), borderBoxRect().height());
}
