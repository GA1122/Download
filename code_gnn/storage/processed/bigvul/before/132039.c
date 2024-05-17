IntRect alignSelectionRectToDevicePixels(LayoutRect& rect)
{
    LayoutUnit roundedX = rect.x().round();
    return IntRect(roundedX, rect.y().round(),
        (rect.maxX() - roundedX).round(),
        snapSizeToPixel(rect.height(), rect.y()));
}
