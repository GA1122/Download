void Document::setDocType(PassRefPtr<DocumentType> docType)
{
    ASSERT(!m_docType || !docType);
    m_docType = docType;
    if (m_docType) {
        this->adoptIfNeeded(m_docType.get());
#if ENABLE(LEGACY_VIEWPORT_ADAPTION)
        if (m_docType->publicId().startsWith("-//wapforum//dtd xhtml mobile 1.",   false))
            processViewport("width=device-width, height=device-height", ViewportArguments::XHTMLMobileProfile);
#endif
    }
    clearStyleResolver();
}
