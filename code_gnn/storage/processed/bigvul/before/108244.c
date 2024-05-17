void FrameLoader::didChangeIcons(DocumentLoader* loader)
{
    if (loader == m_documentLoader)
        m_client->dispatchDidChangeIcons();
}
