void UpdateAtlas::buildLayoutIfNeeded()
{
    if (!m_areaAllocator) {
        m_areaAllocator = adoptPtr(new GeneralAreaAllocator(size()));
        m_areaAllocator->setMinimumAllocation(IntSize(32, 32));
    }
}
