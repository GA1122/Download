void SVGImage::draw(GraphicsContext* context, const FloatRect& dstRect, const FloatRect& srcRect, CompositeOperator compositeOp, blink::WebBlendMode blendMode)
{
    if (!m_page)
        return;

    GraphicsContextStateSaver stateSaver(*context);
    context->setCompositeOperation(compositeOp, blendMode);
    context->clip(enclosingIntRect(dstRect));

    bool compositingRequiresTransparencyLayer = compositeOp != CompositeSourceOver || blendMode != blink::WebBlendModeNormal;
    float opacity = context->getNormalizedAlpha() / 255.f;
    bool requiresTransparencyLayer = compositingRequiresTransparencyLayer || opacity < 1;
    if (requiresTransparencyLayer) {
        context->beginTransparencyLayer(opacity);
        if (compositingRequiresTransparencyLayer)
            context->setCompositeOperation(CompositeSourceOver, blink::WebBlendModeNormal);
    }

    FloatSize scale(dstRect.width() / srcRect.width(), dstRect.height() / srcRect.height());

    FloatSize topLeftOffset(srcRect.location().x() * scale.width(), srcRect.location().y() * scale.height());
    FloatPoint destOffset = dstRect.location() - topLeftOffset;

    context->translate(destOffset.x(), destOffset.y());
    context->scale(scale);

    FrameView* view = frameView();
    view->resize(containerSize());

    if (view->needsLayout())
        view->layout();

    view->paint(context, enclosingIntRect(srcRect));

    if (requiresTransparencyLayer)
        context->endLayer();

    stateSaver.restore();

    if (imageObserver())
        imageObserver()->didDraw(this);
}
