void Scrollbar::setPressedPart(ScrollbarPart part)
{
    if (m_pressedPart != NoPart)
        theme()->invalidatePart(this, m_pressedPart);
    m_pressedPart = part;
    if (m_pressedPart != NoPart)
        theme()->invalidatePart(this, m_pressedPart);
    else if (m_hoveredPart != NoPart)   
        theme()->invalidatePart(this, m_hoveredPart);
}
