bool LayerWebKitThread::startAnimations(double time)
{
    bool didStartAnimations = false;
    for (size_t i = 0; i < m_runningAnimations.size(); ++i) {
        if (!m_runningAnimations[i]->startTime()) {
            m_runningAnimations[i]->setStartTime(time);
            m_didStartAnimations = didStartAnimations = true;
        }
    }

    size_t listSize = m_sublayers.size();
    for (size_t i = 0; i < listSize; ++i)
        didStartAnimations |= m_sublayers[i]->startAnimations(time);

    listSize = m_overlays.size();
    for (size_t i = 0; i < listSize; ++i)
        didStartAnimations |= m_overlays[i]->startAnimations(time);

    return didStartAnimations;
}
