void HarfBuzzShaper::setExpansion(float padding)
{
    m_expansion = padding;
    if (!m_expansion)
        return;

    bool isAfterExpansion = m_isAfterExpansion;
    m_expansionOpportunityCount = Character::expansionOpportunityCount(m_normalizedBuffer.get(), m_normalizedBufferLength, m_run.direction(), isAfterExpansion, m_run.textJustify());
    if (isAfterExpansion && !m_run.allowsTrailingExpansion()) {
        ASSERT(m_expansionOpportunityCount > 0);
        --m_expansionOpportunityCount;
    }

    if (m_expansionOpportunityCount)
        m_expansionPerOpportunity = m_expansion / m_expansionOpportunityCount;
    else
        m_expansionPerOpportunity = 0;
}
