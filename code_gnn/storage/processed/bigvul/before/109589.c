void Document::clearAXObjectCache()
{
    ASSERT(topDocument() == this);
    m_axObjectCache.clear();
}
