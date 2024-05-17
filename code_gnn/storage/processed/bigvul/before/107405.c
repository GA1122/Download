void Scrollbar::startTimerIfNeeded(double delay)
{
    if (m_pressedPart == ThumbPart)
        return;

    if ((m_pressedPart == BackTrackPart || m_pressedPart == ForwardTrackPart) && thumbUnderMouse(this)) {
        theme()->invalidatePart(this, m_pressedPart);
        setHoveredPart(ThumbPart);
        return;
    }

    ScrollDirection dir = pressedPartScrollDirection();
    if (dir == ScrollUp || dir == ScrollLeft) {
        if (m_currentPos == 0)
            return;
    } else {
        if (m_currentPos == maximum())
            return;
    }

    m_scrollTimer.startOneShot(delay);
}
