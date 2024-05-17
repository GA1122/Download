bool ExecutableAllocator::underMemoryPressure()
{
    MetaAllocator::Statistics statistics = allocator->currentStatistics();
    return statistics.bytesAllocated > statistics.bytesReserved / 2;
}
