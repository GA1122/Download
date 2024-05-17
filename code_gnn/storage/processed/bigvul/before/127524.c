void LayerWebKitThread::releaseLayerResources()
{
    deleteTextures();

    size_t listSize = m_sublayers.size();
    for (size_t i = 0; i < listSize; ++i)
        m_sublayers[i]->releaseLayerResources();

    listSize = m_overlays.size();
    for (size_t i = 0; i < listSize; ++i)
        m_overlays[i]->releaseLayerResources();

    if (maskLayer())
        maskLayer()->releaseLayerResources();

    if (replicaLayer())
        replicaLayer()->releaseLayerResources();
}
