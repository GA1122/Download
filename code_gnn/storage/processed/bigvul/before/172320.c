VideoHeap::VideoHeap(int fd, size_t size, void* base)
{
    init(dup(fd), base, size, 0 , MEM_DEVICE);
}
