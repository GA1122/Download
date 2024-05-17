bool TestingDiscardableMemory::lock()
{
    ASSERT(!m_isLocked);
    m_isLocked = true;
    return false;
}
