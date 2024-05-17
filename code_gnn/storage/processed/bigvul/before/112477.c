bool Document::haveStylesheetsLoaded() const
{
    return !m_styleSheetCollection->hasPendingSheets() || m_ignorePendingStylesheets;
}
