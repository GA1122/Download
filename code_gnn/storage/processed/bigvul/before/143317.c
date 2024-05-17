void Frame::disconnectOwnerElement()
{
    if (m_owner) {
        m_owner->clearContentFrame();
        m_owner = nullptr;
    }
}
