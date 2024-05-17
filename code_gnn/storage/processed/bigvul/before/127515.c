void LayerWebKitThread::commitOnCompositingThread()
{
    FloatPoint oldPosition = m_position;
    m_position += m_absoluteOffset;
    replicate(m_layerCompositingThread.get());
#if ENABLE(CSS_FILTERS)
    if (m_filtersChanged) {
        m_filtersChanged = false;
        m_layerCompositingThread->setFilterOperationsChanged(true);
    }
#endif
    if (m_animationsChanged) {
        m_layerCompositingThread->setRunningAnimations(m_runningAnimations);
        m_layerCompositingThread->setSuspendedAnimations(m_suspendedAnimations);
        m_animationsChanged = false;
    }
    if (m_clearOverrideOnCommit) {
        m_layerCompositingThread->clearOverride();
        m_clearOverrideOnCommit = false;
    }
    m_position = oldPosition;
    updateLayerHierarchy();

    commitPendingTextureUploads();

    size_t listSize = m_sublayers.size();
    for (size_t i = 0; i < listSize; ++i)
        m_sublayers[i]->commitOnCompositingThread();

    listSize = m_overlays.size();
    for (size_t i = 0; i < listSize; ++i)
        m_overlays[i]->commitOnCompositingThread();

    if (maskLayer()) {
        maskLayer()->commitOnCompositingThread();
        layerCompositingThread()->setMaskLayer(maskLayer()->layerCompositingThread());
    } else
        layerCompositingThread()->setMaskLayer(0);

    if (replicaLayer()) {
        replicaLayer()->commitOnCompositingThread();
        layerCompositingThread()->setReplicaLayer(replicaLayer()->layerCompositingThread());
    } else
        layerCompositingThread()->setReplicaLayer(0);
}
