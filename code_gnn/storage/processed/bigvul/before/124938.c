bool RenderBox::repaintLayerRectsForImage(WrappedImagePtr image, const FillLayer* layers, bool drawingBackground)
{
    LayoutRect rendererRect;
    RenderBox* layerRenderer = 0;

    for (const FillLayer* curLayer = layers; curLayer; curLayer = curLayer->next()) {
        if (curLayer->image() && image == curLayer->image()->data() && curLayer->image()->canRender(*this, style()->effectiveZoom())) {
            if (!layerRenderer) {
                bool drawingRootBackground = drawingBackground && (isDocumentElement() || (isBody() && !document().documentElement()->renderer()->hasBackground()));
                if (drawingRootBackground) {
                    layerRenderer = view();

                    LayoutUnit rw;
                    LayoutUnit rh;

                    if (FrameView* frameView = toRenderView(layerRenderer)->frameView()) {
                        rw = frameView->contentsWidth();
                        rh = frameView->contentsHeight();
                    } else {
                        rw = layerRenderer->width();
                        rh = layerRenderer->height();
                    }
                    rendererRect = LayoutRect(-layerRenderer->marginLeft(),
                        -layerRenderer->marginTop(),
                        max(layerRenderer->width() + layerRenderer->marginWidth() + layerRenderer->borderLeft() + layerRenderer->borderRight(), rw),
                        max(layerRenderer->height() + layerRenderer->marginHeight() + layerRenderer->borderTop() + layerRenderer->borderBottom(), rh));
                } else {
                    layerRenderer = this;
                    rendererRect = borderBoxRect();
                }
            }

            BackgroundImageGeometry geometry;
            layerRenderer->calculateBackgroundImageGeometry(0, curLayer, rendererRect, geometry);
            if (geometry.hasNonLocalGeometry()) {
                layerRenderer->repaint();
                return true;
            }

            layerRenderer->repaintRectangle(geometry.destRect());
            if (geometry.destRect() == rendererRect)
                return true;
        }
    }
    return false;
}
