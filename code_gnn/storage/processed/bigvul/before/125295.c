DocumentTimeline::DocumentTimeline(Document* document, PassOwnPtr<PlatformTiming> timing)
    : m_zeroTime(nullValue())
    , m_document(document)
    , m_eventDistpachTimer(this, &DocumentTimeline::eventDispatchTimerFired)
{
    if (!timing)
        m_timing = adoptPtr(new DocumentTimelineTiming(this));
    else
        m_timing = timing;

    ASSERT(document);
}
