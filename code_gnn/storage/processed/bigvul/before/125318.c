bool Player::hasActiveAnimationsOnCompositor()
{
    if (!m_content || !m_content->isAnimation())
        return false;
    return toAnimation(m_content.get())->hasActiveAnimationsOnCompositor();
}
