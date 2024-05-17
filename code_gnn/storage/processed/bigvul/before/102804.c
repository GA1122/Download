bool ExecutableAllocator::isValid() const
{
    return !!allocator->bytesReserved();
}
