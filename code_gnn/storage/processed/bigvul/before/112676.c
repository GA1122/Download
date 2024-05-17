bool DocumentLoader::maybeCreateArchive()
{
#if !ENABLE(WEB_ARCHIVE) && !ENABLE(MHTML)
    return false;
#else
    
    RefPtr<ResourceBuffer> mainResourceBuffer = mainResourceData();
    m_archive = ArchiveFactory::create(m_response.url(), mainResourceBuffer ? mainResourceBuffer->sharedBuffer() : 0, m_response.mimeType());
    if (!m_archive)
        return false;
    
    addAllArchiveResources(m_archive.get());
    ArchiveResource* mainResource = m_archive->mainResource();
    m_parsedArchiveData = mainResource->data();
    m_writer.setMIMEType(mainResource->mimeType());
    
    ASSERT(m_frame->document());
    commitData(mainResource->data()->data(), mainResource->data()->size());
    return true;
#endif  
}
