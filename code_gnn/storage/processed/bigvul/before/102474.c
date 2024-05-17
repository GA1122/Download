void LayerTreeCoordinator::renderNextFrame()
{
    m_waitingForUIProcess = false;
    scheduleLayerFlush();
    for (unsigned i = 0; i < m_updateAtlases.size(); ++i)
        m_updateAtlases[i]->didSwapBuffers();
}
