void WebPagePrivate::suspendRootLayerCommit()
{
    if (m_suspendRootLayerCommit)
        return;

    m_suspendRootLayerCommit = true;

    if (!m_compositor)
        return;

    releaseLayerResources();
}
