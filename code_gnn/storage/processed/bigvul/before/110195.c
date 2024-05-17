float DateTimeSymbolicFieldElement::maximumWidth(const Font& font)
{
    float maximumWidth = font.width(visibleEmptyValue());
    for (unsigned index = 0; index < m_symbols.size(); ++index)
        maximumWidth = std::max(maximumWidth, font.width(m_symbols[index]));
    return maximumWidth + DateTimeFieldElement::maximumWidth(font);
}
