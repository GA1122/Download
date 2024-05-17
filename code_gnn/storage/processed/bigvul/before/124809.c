bool RenderBox::backgroundIsKnownToBeOpaqueInRect(const LayoutRect& localRect) const
{
    if (isBody() && skipBodyBackground(this))
        return false;

    Color backgroundColor = resolveColor(CSSPropertyBackgroundColor);
    if (backgroundColor.hasAlpha())
        return false;

    if (style()->hasAppearance())
        return false;

    if (style()->hasBorderRadius())
        return false;
    if (style()->backgroundLayers()->next())
        return false;
    LayoutRect backgroundRect;
    switch (style()->backgroundClip()) {
    case BorderFillBox:
        backgroundRect = borderBoxRect();
        break;
    case PaddingFillBox:
        backgroundRect = paddingBoxRect();
        break;
    case ContentFillBox:
        backgroundRect = contentBoxRect();
        break;
    default:
        break;
    }
    return backgroundRect.contains(localRect);
}
