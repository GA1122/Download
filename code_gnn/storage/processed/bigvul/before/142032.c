void TextTrackLoader::cueLoadTimerFired(TimerBase* timer)
{
    DCHECK_EQ(timer, &m_cueLoadTimer);

    if (m_newCuesAvailable) {
        m_newCuesAvailable = false;
        m_client->newCuesAvailable(this);
    }

    if (m_state >= Finished)
        m_client->cueLoadingCompleted(this, m_state == Failed);
}
