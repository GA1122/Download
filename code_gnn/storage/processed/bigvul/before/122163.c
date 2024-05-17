void RenderLayerCompositor::layerWasAdded(RenderLayer*  , RenderLayer*  )
{
    setCompositingLayersNeedRebuild();
}
