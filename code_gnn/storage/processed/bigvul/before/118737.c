void HTMLDocument::removeNamedItem(const AtomicString& name)
{
    removeItemFromMap(m_namedItemCounts, name);
}
