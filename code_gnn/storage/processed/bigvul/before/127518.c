PassRefPtr<LayerWebKitThread> LayerWebKitThread::create(LayerType type, GraphicsLayerBlackBerry* owner)
{
    return adoptRef(new LayerWebKitThread(type, owner));
}
