void LayerWebKitThread::insert(Vector<RefPtr<LayerWebKitThread> >& list, PassRefPtr<LayerWebKitThread> sublayer, size_t index)
{
    sublayer->removeFromSuperlayer();
    index = min(index, list.size());
    sublayer->setSuperlayer(this);
    list.insert(index, sublayer);

    setNeedsCommit();
}
