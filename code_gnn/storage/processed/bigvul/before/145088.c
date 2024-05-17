void TestingDiscardableMemory::unlock()
{
    ASSERT(m_isLocked);
    m_isLocked = false;
    memset(m_data.data(), 0, m_data.size());
}
