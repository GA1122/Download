PassRefPtr<RenderStyle> BaseMultipleFieldsDateAndTimeInputType::customStyleForRenderer(PassRefPtr<RenderStyle> originalStyle)
{
    EDisplay originalDisplay = originalStyle->display();
    EDisplay newDisplay = originalDisplay;
    if (originalDisplay == INLINE || originalDisplay == INLINE_BLOCK)
        newDisplay = INLINE_FLEX;
    else if (originalDisplay == BLOCK)
        newDisplay = FLEX;
    TextDirection contentDirection = element()->locale().isRTL() ? RTL : LTR;
    if (originalStyle->direction() == contentDirection && originalDisplay == newDisplay)
        return originalStyle;

    RefPtr<RenderStyle> style = RenderStyle::clone(originalStyle.get());
    style->setDirection(contentDirection);
    style->setDisplay(newDisplay);
    return style.release();
}
