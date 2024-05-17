void DateTimeSymbolicFieldElement::stepUp()
{
    if (hasValue()) {
        if (!indexIsInRange(++m_selectedIndex))
            m_selectedIndex = m_minimumIndex;
    } else
        m_selectedIndex = m_minimumIndex;
    updateVisibleValue(DispatchEvent);
}
