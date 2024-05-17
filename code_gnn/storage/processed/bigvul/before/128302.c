void FrameView::recalculateScrollbarOverlayStyle()
{
    ScrollbarOverlayStyle oldOverlayStyle = scrollbarOverlayStyle();
    ScrollbarOverlayStyle overlayStyle = ScrollbarOverlayStyleDefault;

    Color backgroundColor = documentBackgroundColor();
    double hue, saturation, lightness;
    backgroundColor.getHSL(hue, saturation, lightness);
    if (lightness <= .5)
        overlayStyle = ScrollbarOverlayStyleLight;

    if (oldOverlayStyle != overlayStyle)
        setScrollbarOverlayStyle(overlayStyle);
}
