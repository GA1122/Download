void RenderBox::addLayerHitTestRects(LayerHitTestRects& layerRects, const RenderLayer* currentLayer, const LayoutPoint& layerOffset, const LayoutRect& containerRect) const
{
    LayoutPoint adjustedLayerOffset = layerOffset + locationOffset();
    RenderBoxModelObject::addLayerHitTestRects(layerRects, currentLayer, adjustedLayerOffset, containerRect);
}
