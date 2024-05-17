void DocumentTimeline::setZeroTime(double zeroTime)
{
    ASSERT(isNull(m_zeroTime));
    m_zeroTime = zeroTime;
    ASSERT(!isNull(m_zeroTime));
    serviceAnimations();
}
