const LayerWebKitThread* LayerWebKitThread::rootLayer() const
{
    const LayerWebKitThread* layer = this;
    LayerWebKitThread* superlayer = layer->superlayer();

    while (superlayer) {
        layer = superlayer;
        superlayer = superlayer->superlayer();
    }
    return layer;
}
