void HTMLAnchorElement::setRel(const AtomicString& value)
{
    m_linkRelations = 0;
    SpaceSplitString newLinkRelations(value, SpaceSplitString::ShouldFoldCase);
    if (newLinkRelations.contains("noreferrer"))
        m_linkRelations |= RelationNoReferrer;
    if (newLinkRelations.contains("noopener"))
        m_linkRelations |= RelationNoOpener;
}
