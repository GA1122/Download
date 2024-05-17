Player::~Player()
{
    if (m_content)
        m_content->detach();
}
