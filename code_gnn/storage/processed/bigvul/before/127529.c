void LayerWebKitThread::replaceSublayer(LayerWebKitThread* reference, PassRefPtr<LayerWebKitThread> newLayer)
{
    ASSERT_ARG(reference, reference);
    ASSERT_ARG(reference, reference->superlayer() == this);

    if (reference == newLayer)
        return;

    int referenceIndex = m_sublayers.find(reference);
    if (referenceIndex == notFound) {
        ASSERT_NOT_REACHED();
        return;
    }

    reference->removeFromSuperlayer();

    if (newLayer) {
        newLayer->removeFromSuperlayer();
        insertSublayer(newLayer, referenceIndex);
    }
}
