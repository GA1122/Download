void StyleResolver::addToStyleSharingList(Element& element)
{
    if (!document().inStyleRecalc())
        return;
    INCREMENT_STYLE_STATS_COUNTER(*this, sharedStyleCandidates);
    if (m_styleSharingList.size() >= styleSharingListSize)
        m_styleSharingList.remove(--m_styleSharingList.end());
    m_styleSharingList.prepend(&element);
}
