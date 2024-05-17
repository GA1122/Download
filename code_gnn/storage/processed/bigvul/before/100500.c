unsigned Cache::deadCapacity() const 
{
    unsigned capacity = m_capacity - min(m_liveSize, m_capacity);  
    capacity = max(capacity, m_minDeadCapacity);  
    capacity = min(capacity, m_maxDeadCapacity);  
    return capacity;
}
