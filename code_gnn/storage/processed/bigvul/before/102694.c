void CCLayerTreeHost::updateCompositorResources(LayerChromium* layer, GraphicsContext3D* context, TextureAllocator* allocator)
{
    if (layer->bounds().isEmpty())
        return;

    if (!layer->opacity())
        return;

    if (layer->maskLayer())
        updateCompositorResources(layer->maskLayer(), context, allocator);
    if (layer->replicaLayer())
        updateCompositorResources(layer->replicaLayer(), context, allocator);

    if (layer->drawsContent())
        layer->updateCompositorResources(context, allocator);
}
