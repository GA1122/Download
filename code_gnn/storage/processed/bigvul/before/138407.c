void Document::clearWeakMembers(Visitor* visitor)
{
    if (m_axObjectCache)
        m_axObjectCache->clearWeakMembers(visitor);
}
