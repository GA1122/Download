size_t DisplayItemList::approximateUnsharedMemoryUsage() const
{
    size_t memoryUsage = sizeof(*this);

    memoryUsage += m_currentDisplayItems.memoryUsageInBytes();


    ASSERT(m_newDisplayItems.isEmpty());
    memoryUsage += m_newDisplayItems.memoryUsageInBytes();

    return memoryUsage;
}
