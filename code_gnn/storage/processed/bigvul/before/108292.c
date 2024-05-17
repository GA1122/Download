void FrameLoader::willSetEncoding()
{
    if (!m_workingURL.isEmpty())
        receivedFirstData();
}
