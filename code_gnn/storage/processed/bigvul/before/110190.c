DateTimeSymbolicFieldElement::DateTimeSymbolicFieldElement(Document* document, FieldOwner& fieldOwner, const Vector<String>& symbols, int minimum, int maximum)
    : DateTimeFieldElement(document, fieldOwner)
    , m_symbols(symbols)
    , m_visibleEmptyValue(makeVisibleEmptyValue(symbols))
    , m_selectedIndex(-1)
    , m_typeAhead(this)
    , m_minimumIndex(minimum)
    , m_maximumIndex(maximum)
{
    ASSERT(!symbols.isEmpty());
    ASSERT(m_minimumIndex >= 0);
    ASSERT(m_maximumIndex < static_cast<int>(m_symbols.size()));
    ASSERT(m_minimumIndex <= m_maximumIndex);
}
