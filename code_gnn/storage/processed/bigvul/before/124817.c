void RenderBox::clearLayoutOverflow()
{
    if (!m_overflow)
        return;

    if (!hasVisualOverflow() && contentsVisualOverflowRect().isEmpty()) {
        m_overflow.clear();
        return;
    }

    m_overflow->setLayoutOverflow(noOverflowRect());
}
