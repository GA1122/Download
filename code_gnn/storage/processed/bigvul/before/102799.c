PassRefPtr<ExecutableMemoryHandle> ExecutableAllocator::allocate(JSGlobalData& globalData, size_t sizeInBytes, void* ownerUID, JITCompilationEffort effort)
{
    RefPtr<ExecutableMemoryHandle> result = allocator->allocate(sizeInBytes, ownerUID);
    if (!result) {
        if (effort == JITCompilationCanFail)
            return result;
        releaseExecutableMemory(globalData);
        result = allocator->allocate(sizeInBytes, ownerUID);
        if (!result)
            CRASH();
    }
    return result.release();
}
