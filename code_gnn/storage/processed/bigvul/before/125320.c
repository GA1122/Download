bool Player::maybeStartAnimationOnCompositor()
{
    ASSERT(!hasStartTime());
    if (!m_content || !m_content->isAnimation())
        return false;

    return toAnimation(m_content.get())->maybeStartAnimationOnCompositor();
}
