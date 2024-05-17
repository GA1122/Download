void CCLayerTreeHost::didRecreateGraphicsContext(bool success)
{
    if (rootLayer())
        rootLayer()->cleanupResourcesRecursive();
    m_client->didRecreateGraphicsContext(success);
}
