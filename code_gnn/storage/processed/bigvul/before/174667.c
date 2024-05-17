size_t IMemory::size() const {
 size_t size;
    getMemory(NULL, &size);
 return size;
}
