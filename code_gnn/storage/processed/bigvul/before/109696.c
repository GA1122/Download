bool Document::haveImportsLoaded() const
{
    return !m_import || !m_import->isBlocked();
}
