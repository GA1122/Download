void DateTimeSymbolicFieldElement::stepDown()
{
    if (hasValue()) {
        if (!indexIsInRange(--m_selectedIndex))
            m_selectedIndex = m_maximumIndex;
    } else
        m_selectedIndex = m_maximumIndex;
    updateVisibleValue(DispatchEvent);
}
