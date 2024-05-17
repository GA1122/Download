void Scrollbar::setHoveredPart(ScrollbarPart part)
{
    if (part == m_hoveredPart)
        return;

    if ((m_hoveredPart == NoPart || part == NoPart) && theme()->invalidateOnMouseEnterExit())
        invalidate();   
    else if (m_pressedPart == NoPart) {   
        theme()->invalidatePart(this, part);
        theme()->invalidatePart(this, m_hoveredPart);
    }
    m_hoveredPart = part;
}
