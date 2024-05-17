void RenderLayerScrollableArea::updateCompositingLayersAfterScroll()
{
    RenderLayerCompositor* compositor = box().view()->compositor();
    if (compositor->inCompositingMode()) {
        if (usesCompositedScrolling()) {
            DisableCompositingQueryAsserts disabler;
            ASSERT(layer()->hasCompositedLayerMapping());
            layer()->compositedLayerMapping()->setNeedsGraphicsLayerUpdate(GraphicsLayerUpdateSubtree);
            compositor->setNeedsCompositingUpdate(CompositingUpdateAfterGeometryChange);
        } else {
            layer()->setNeedsCompositingInputsUpdate();
        }
    }
}
