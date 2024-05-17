WebDiscardableMemory* TestingPlatformSupport::allocateAndLockDiscardableMemory(size_t bytes)
{
    return !m_config.hasDiscardableMemorySupport ? 0 : new TestingDiscardableMemory(bytes);
}
