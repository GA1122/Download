void HTMLDocument::removeExtraNamedItem(const AtomicString& name)
{
    removeItemFromMap(m_extraNamedItemCounts, name);
}
