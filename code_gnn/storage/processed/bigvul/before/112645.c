void DocumentLoader::commitData(const char* bytes, size_t length)
{
    if (!m_gotFirstByte) {
        m_gotFirstByte = true;
        m_writer.begin(documentURL(), false);
        m_writer.setDocumentWasLoadedAsPartOfNavigation();

        if (SecurityPolicy::allowSubstituteDataAccessToLocal() && m_originalSubstituteDataWasValid) {
            m_frame->document()->securityOrigin()->grantLoadLocalResources();
        }

        if (frameLoader()->stateMachine()->creatingInitialEmptyDocument())
            return;
        
#if ENABLE(MHTML)
        if (m_archive && m_archive->type() == Archive::MHTML)
            m_frame->document()->setBaseURLOverride(m_archive->mainResource()->url());
#endif

        if (!isMultipartReplacingLoad())
            frameLoader()->receivedFirstData();

        bool userChosen = true;
        String encoding = overrideEncoding();
        if (encoding.isNull()) {
            userChosen = false;
            encoding = response().textEncodingName();
#if ENABLE(WEB_ARCHIVE)
            if (m_archive && m_archive->type() == Archive::WebArchive)
                encoding = m_archive->mainResource()->textEncoding();
#endif
        }
        m_writer.setEncoding(encoding, userChosen);
    }
    ASSERT(m_frame->document()->parsing());
    m_writer.addData(bytes, length);
}
