void DocumentLoader::addAllArchiveResources(Archive* archive)
{
    if (!m_archiveResourceCollection)
        m_archiveResourceCollection = adoptPtr(new ArchiveResourceCollection);
        
    ASSERT(archive);
    if (!archive)
        return;
        
    m_archiveResourceCollection->addAllResources(archive);
}
