void* TestingDiscardableMemory::data()
{
    ASSERT(m_isLocked);
    return m_data.data();
}
