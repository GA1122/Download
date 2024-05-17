void Document::setImportsController(HTMLImportsController* controller)
{
    DCHECK(!m_importsController || !controller);
    m_importsController = controller;
    if (!m_importsController && !loader())
        m_fetcher->clearContext();
}
