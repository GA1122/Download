bool Scrollbar::mouseDown(const PlatformMouseEvent& evt)
{
    if (evt.button() == RightButton)
        return true;  

    setPressedPart(theme()->hitTest(this, evt));
    int pressedPos = (orientation() == HorizontalScrollbar ? convertFromContainingWindow(evt.pos()).x() : convertFromContainingWindow(evt.pos()).y());
    
    if ((m_pressedPart == BackTrackPart || m_pressedPart == ForwardTrackPart) && theme()->shouldCenterOnThumb(this, evt)) {
        setHoveredPart(ThumbPart);
        setPressedPart(ThumbPart);
        m_dragOrigin = m_currentPos;
        int thumbLen = theme()->thumbLength(this);
        int desiredPos = pressedPos;
        m_pressedPos = theme()->trackPosition(this) + theme()->thumbPosition(this) + thumbLen / 2;
        moveThumb(desiredPos);
        return true;
    } else if (m_pressedPart == ThumbPart)
        m_dragOrigin = m_currentPos;
    
    m_pressedPos = pressedPos;

    autoscrollPressedPart(theme()->initialAutoscrollTimerDelay());
    return true;
}
