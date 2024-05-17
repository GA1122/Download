static uint64_t ahci_mem_read(void *opaque, hwaddr addr, unsigned size)
{
    hwaddr aligned = addr & ~0x3;
    int ofst = addr - aligned;
    uint64_t lo = ahci_mem_read_32(opaque, aligned);
    uint64_t hi;
    uint64_t val;

     
    if (ofst + size <= 4) {
        val = lo >> (ofst * 8);
    } else {
        g_assert_cmpint(size, >, 1);

         
        hi = ahci_mem_read_32(opaque, aligned + 4);
        val = (hi << 32 | lo) >> (ofst * 8);
    }

    DPRINTF(-1, "addr=0x%" HWADDR_PRIx " val=0x%" PRIx64 ", size=%d\n",
            addr, val, size);
    return val;
}
