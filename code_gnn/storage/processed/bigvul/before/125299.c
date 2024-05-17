double DocumentTimeline::currentTime()
{
    return m_document->animationClock().currentTime() - m_zeroTime;
}
