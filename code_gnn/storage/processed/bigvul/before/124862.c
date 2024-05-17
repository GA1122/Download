BackgroundBleedAvoidance RenderBox::determineBackgroundBleedAvoidance(GraphicsContext* context) const
{
    if (context->paintingDisabled())
        return BackgroundBleedNone;

    const RenderStyle* style = this->style();

    if (!style->hasBackground() || !style->hasBorder() || !style->hasBorderRadius() || borderImageIsLoadedAndCanBeRendered())
        return BackgroundBleedNone;

    AffineTransform ctm = context->getCTM();
    FloatSize contextScaling(static_cast<float>(ctm.xScale()), static_cast<float>(ctm.yScale()));

    if (contextScaling.width() > 1)
        contextScaling.setWidth(1);
    if (contextScaling.height() > 1)
        contextScaling.setHeight(1);

    if (borderObscuresBackgroundEdge(contextScaling))
        return BackgroundBleedShrinkBackground;
    if (!style->hasAppearance() && borderObscuresBackground() && backgroundHasOpaqueTopLayer())
        return BackgroundBleedBackgroundOverBorder;

    return BackgroundBleedClipBackground;
}
