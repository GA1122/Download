Player::Player(DocumentTimeline& timeline, TimedItem* content)
    : m_playbackRate(1)
    , m_startTime(nullValue())
    , m_holdTime(nullValue())
    , m_storedTimeLag(0)
    , m_content(content)
    , m_timeline(timeline)
    , m_paused(false)
    , m_held(false)
    , m_isPausedForTesting(false)
{
    if (m_content)
        m_content->attach(this);
}
