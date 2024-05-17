void Document::setDoctype(PassRefPtr<DocumentType> docType)
{
    ASSERT(!m_docType || !docType);
    m_docType = docType;
    if (m_docType) {
        this->adoptIfNeeded(m_docType.get());
        if (m_docType->publicId().startsWith("-//wapforum//dtd xhtml mobile 1.",   false))
            m_isMobileDocument = true;
    }
    clearStyleResolver();
}
