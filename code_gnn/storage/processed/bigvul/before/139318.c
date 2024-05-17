float HarfBuzzShaper::nextExpansionPerOpportunity()
{
    if (!m_expansionOpportunityCount) {
        ASSERT_NOT_REACHED();  
        return 0;
    }
    if (!--m_expansionOpportunityCount) {
        float remaining = m_expansion;
        m_expansion = 0;
        return remaining;
    }
    m_expansion -= m_expansionPerOpportunity;
    return m_expansionPerOpportunity;
}
