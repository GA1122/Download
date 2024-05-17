bool Document::haveStylesheetsLoaded() const
{
    return !m_styleEngine->hasPendingSheets() || m_ignorePendingStylesheets;
}
