PassRefPtr<Archive> DocumentLoader::popArchiveForSubframe(const String& frameName, const KURL& url)
{
    return m_archiveResourceCollection ? m_archiveResourceCollection->popSubframeArchive(frameName, url) : PassRefPtr<Archive>(0);
}
