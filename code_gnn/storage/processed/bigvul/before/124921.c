void RenderBox::paintMaskImages(const PaintInfo& paintInfo, const LayoutRect& paintRect)
{
    bool pushTransparencyLayer = false;
    bool compositedMask = hasLayer() && layer()->hasCompositedMask();
    bool flattenCompositingLayers = view()->frameView() && view()->frameView()->paintBehavior() & PaintBehaviorFlattenCompositingLayers;
    CompositeOperator compositeOp = CompositeSourceOver;

    bool allMaskImagesLoaded = true;

    if (!compositedMask || flattenCompositingLayers) {
        pushTransparencyLayer = true;
        StyleImage* maskBoxImage = style()->maskBoxImage().image();
        const FillLayer* maskLayers = style()->maskLayers();

        if (maskBoxImage)
            allMaskImagesLoaded &= maskBoxImage->isLoaded();

        if (maskLayers)
            allMaskImagesLoaded &= maskLayers->imagesAreLoaded();

        paintInfo.context->setCompositeOperation(CompositeDestinationIn);
        paintInfo.context->beginTransparencyLayer(1);
        compositeOp = CompositeSourceOver;
    }

    if (allMaskImagesLoaded) {
        paintFillLayers(paintInfo, Color::transparent, style()->maskLayers(), paintRect, BackgroundBleedNone, compositeOp);
        paintNinePieceImage(paintInfo.context, paintRect, style(), style()->maskBoxImage(), compositeOp);
    }

    if (pushTransparencyLayer)
        paintInfo.context->endLayer();
}
