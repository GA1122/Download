void LayerWebKitThread::removeAll(Vector<RefPtr<LayerWebKitThread> >& vector)
{
    if (!vector.size())
        return;

    while (vector.size()) {
        RefPtr<LayerWebKitThread> layer = vector[0].get();
        ASSERT(layer->superlayer() == this);
        layer->removeFromSuperlayer();
    }

    setNeedsCommit();
}
