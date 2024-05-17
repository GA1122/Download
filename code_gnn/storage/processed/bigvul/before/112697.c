void DocumentLoader::setArchive(PassRefPtr<Archive> archive)
{
    m_archive = archive;
    addAllArchiveResources(m_archive.get());
}
