void ExecutableAllocator::initializeAllocator()
{
    ASSERT(!allocator);
    allocator = new FixedVMPoolExecutableAllocator();
    CodeProfiling::notifyAllocator(allocator);
}
