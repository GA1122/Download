bool DocumentLoader::scheduleArchiveLoad(ResourceLoader* loader, const ResourceRequest& request)
{
    if (ArchiveResource* resource = archiveResourceForURL(request.url())) {
        m_pendingSubstituteResources.set(loader, resource);
        deliverSubstituteResourcesAfterDelay();
        return true;
    }

    if (!m_archive)
        return false;

    switch (m_archive->type()) {
#if ENABLE(WEB_ARCHIVE)
    case Archive::WebArchive:
        return m_frame->settings() && m_frame->settings()->webArchiveDebugModeEnabled() && ArchiveFactory::isArchiveMimeType(responseMIMEType());
#endif
#if ENABLE(MHTML)
    case Archive::MHTML:
        return true;  
#endif
    default:
        return false;
    }
}
