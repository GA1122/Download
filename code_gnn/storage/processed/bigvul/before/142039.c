void TextTrackLoader::notifyFinished(Resource* resource)
{
    DCHECK_EQ(this->resource(), resource);
    if (m_state != Failed)
        m_state = resource->errorOccurred() ? Failed : Finished;

    if (m_state == Finished && m_cueParser)
        m_cueParser->flush();

    if (!m_cueLoadTimer.isActive())
        m_cueLoadTimer.startOneShot(0, BLINK_FROM_HERE);

    cancelLoad();
}
