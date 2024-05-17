ssize_t IMemory::offset() const {
 ssize_t offset;
    getMemory(&offset);
 return offset;
}
