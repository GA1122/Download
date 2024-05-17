void LayerWebKitThread::remove(Vector<RefPtr<LayerWebKitThread> >& vector, LayerWebKitThread* sublayer)
{
    int foundIndex = vector.find(sublayer);
    if (foundIndex == notFound)
        return;

    sublayer->setSuperlayer(0);
    vector.remove(foundIndex);

    setNeedsCommit();
}
