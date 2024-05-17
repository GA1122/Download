void Document::setImport(HTMLImport* import)
{
    ASSERT(!m_import || !import);
    m_import = import;
}
