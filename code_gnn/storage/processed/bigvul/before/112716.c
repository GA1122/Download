KURL DocumentLoader::urlForHistory() const
{
    if (m_substituteData.isValid())
        return unreachableURL();

    return m_originalRequestCopy.url();
}
