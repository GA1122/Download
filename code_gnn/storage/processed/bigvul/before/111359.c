void WebPagePrivate::setNeedsOneShotDrawingSynchronization()
{
    if (compositorDrawsRootLayer()) {
        scheduleRootLayerCommit();
        return;
    }

    m_needsCommit = true;
    m_needsOneShotDrawingSynchronization = true;
}
