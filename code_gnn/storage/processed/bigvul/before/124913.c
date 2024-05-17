void RenderBox::paintBackground(const PaintInfo& paintInfo, const LayoutRect& paintRect, BackgroundBleedAvoidance bleedAvoidance)
{
    if (isDocumentElement()) {
        paintRootBoxFillLayers(paintInfo);
        return;
    }
    if (isBody() && skipBodyBackground(this))
        return;
    if (backgroundIsKnownToBeObscured())
        return;
    paintFillLayers(paintInfo, resolveColor(CSSPropertyBackgroundColor), style()->backgroundLayers(), paintRect, bleedAvoidance);
}
