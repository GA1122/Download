void RenderBox::paintFillLayers(const PaintInfo& paintInfo, const Color& c, const FillLayer* fillLayer, const LayoutRect& rect,
    BackgroundBleedAvoidance bleedAvoidance, CompositeOperator op, RenderObject* backgroundObject)
{
    Vector<const FillLayer*, 8> layers;
    const FillLayer* curLayer = fillLayer;
    bool shouldDrawBackgroundInSeparateBuffer = false;
    while (curLayer) {
        layers.append(curLayer);

        if (!shouldDrawBackgroundInSeparateBuffer && curLayer->blendMode() != blink::WebBlendModeNormal)
            shouldDrawBackgroundInSeparateBuffer = true;

        if (curLayer->clipOccludesNextLayers(curLayer == fillLayer) && curLayer->hasOpaqueImage(this) && curLayer->image()->canRender(*this, style()->effectiveZoom()) && curLayer->hasRepeatXY() && curLayer->blendMode() == blink::WebBlendModeNormal && !boxShadowShouldBeAppliedToBackground(bleedAvoidance))
            break;
        curLayer = curLayer->next();
    }

    GraphicsContext* context = paintInfo.context;
    if (!context)
        shouldDrawBackgroundInSeparateBuffer = false;
    if (shouldDrawBackgroundInSeparateBuffer)
        context->beginTransparencyLayer(1);

    Vector<const FillLayer*>::const_reverse_iterator topLayer = layers.rend();
    for (Vector<const FillLayer*>::const_reverse_iterator it = layers.rbegin(); it != topLayer; ++it)
        paintFillLayer(paintInfo, c, *it, rect, bleedAvoidance, op, backgroundObject);

    if (shouldDrawBackgroundInSeparateBuffer)
        context->endLayer();
}
