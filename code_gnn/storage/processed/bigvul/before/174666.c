void* IMemory::pointer() const {
 ssize_t offset;
    sp<IMemoryHeap> heap = getMemory(&offset);
 void* const base = heap!=0 ? heap->base() : MAP_FAILED;
 if (base == MAP_FAILED)
 return 0;
 return static_cast<char*>(base) + offset;
}
