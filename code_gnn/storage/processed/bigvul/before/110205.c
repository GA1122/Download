String DateTimeSymbolicFieldElement::visibleValue() const
{
    return hasValue() ? m_symbols[m_selectedIndex] : visibleEmptyValue();
}
