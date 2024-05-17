void DocumentTimeline::DocumentTimelineTiming::wakeAfter(double duration)
{
    m_timer.startOneShot(duration);
}
