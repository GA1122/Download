void HTMLConstructionSite::dispatchDocumentElementAvailableIfNeeded()
{
    if (m_document->frame() && !m_isParsingFragment)
        m_document->frame()->loader()->dispatchDocumentElementAvailable();
}
