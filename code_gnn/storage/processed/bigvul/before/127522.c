 void LayerWebKitThread::notifyAnimationsStarted(double time)
{
    if (m_didStartAnimations) {
        m_didStartAnimations = false;
        if (m_owner)
            m_owner->notifyAnimationStarted(time);
    }

    size_t listSize = m_sublayers.size();
    for (size_t i = 0; i < listSize; ++i)
        m_sublayers[i]->notifyAnimationsStarted(time);

    listSize = m_overlays.size();
    for (size_t i = 0; i < listSize; ++i)
        m_overlays[i]->notifyAnimationsStarted(time);
}
