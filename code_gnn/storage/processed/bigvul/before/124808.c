bool RenderBox::backgroundHasOpaqueTopLayer() const
{
    const FillLayer* fillLayer = style()->backgroundLayers();
    if (!fillLayer || fillLayer->clip() != BorderFillBox)
        return false;

    if (hasOverflowClip() && fillLayer->attachment() == LocalBackgroundAttachment)
        return false;

    if (fillLayer->hasOpaqueImage(this) && fillLayer->hasRepeatXY() && fillLayer->image()->canRender(*this, style()->effectiveZoom()))
        return true;

    if (!fillLayer->next() && !fillLayer->hasImage()) {
        Color bgColor = resolveColor(CSSPropertyBackgroundColor);
        if (bgColor.alpha() == 255)
            return true;
    }

    return false;
}
