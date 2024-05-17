static void updateOffsetFromViewportForSelf(RenderLayer* renderLayer)
{
    RenderLayerBacking* backing = renderLayer->backing();
    if (!backing)
        return;

    RenderStyle* style = renderLayer->renderer()->style();
    if (!style)
        return;

    if (!renderLayer->renderer()->isOutOfFlowPositioned() || renderLayer->renderer()->style()->position() != FixedPosition)
        return;

    if (!renderLayer->renderer()->container()->isRenderView())
        return;

    if (!renderLayer->isStackingContext())
        return;

    CoordinatedGraphicsLayer* graphicsLayer = toCoordinatedGraphicsLayer(backing->graphicsLayer());
    graphicsLayer->setFixedToViewport(true);
}
