static inline void address_space_stw_internal(AddressSpace *as,
                                              hwaddr addr, uint32_t val,
                                              MemTxAttrs attrs,
                                              MemTxResult *result,
                                              enum device_endian endian)
{
    uint8_t *ptr;
    MemoryRegion *mr;
    hwaddr l = 2;
    hwaddr addr1;
    MemTxResult r;
    bool release_lock = false;

    rcu_read_lock();
    mr = address_space_translate(as, addr, &addr1, &l, true);
    if (l < 2 || !memory_access_is_direct(mr, true)) {
        release_lock |= prepare_mmio_access(mr);

#if defined(TARGET_WORDS_BIGENDIAN)
        if (endian == DEVICE_LITTLE_ENDIAN) {
            val = bswap16(val);
        }
#else
        if (endian == DEVICE_BIG_ENDIAN) {
            val = bswap16(val);
        }
#endif
        r = memory_region_dispatch_write(mr, addr1, val, 2, attrs);
    } else {
         
        addr1 += memory_region_get_ram_addr(mr) & TARGET_PAGE_MASK;
        ptr = qemu_get_ram_ptr(addr1);
        switch (endian) {
        case DEVICE_LITTLE_ENDIAN:
            stw_le_p(ptr, val);
            break;
        case DEVICE_BIG_ENDIAN:
            stw_be_p(ptr, val);
            break;
        default:
            stw_p(ptr, val);
            break;
        }
        invalidate_and_set_dirty(mr, addr1, 2);
        r = MEMTX_OK;
    }
    if (result) {
        *result = r;
    }
    if (release_lock) {
        qemu_mutex_unlock_iothread();
    }
    rcu_read_unlock();
}