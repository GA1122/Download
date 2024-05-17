static bool borderOrPaddingLogicalWidthChanged(const RenderStyle* oldStyle, const RenderStyle* newStyle)
{
    if (newStyle->isHorizontalWritingMode())
        return oldStyle->borderLeftWidth() != newStyle->borderLeftWidth()
            || oldStyle->borderRightWidth() != newStyle->borderRightWidth()
            || oldStyle->paddingLeft() != newStyle->paddingLeft()
            || oldStyle->paddingRight() != newStyle->paddingRight();

    return oldStyle->borderTopWidth() != newStyle->borderTopWidth()
        || oldStyle->borderBottomWidth() != newStyle->borderBottomWidth()
        || oldStyle->paddingTop() != newStyle->paddingTop()
        || oldStyle->paddingBottom() != newStyle->paddingBottom();
}
