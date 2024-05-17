MemTxResult address_space_rw(AddressSpace *as, hwaddr addr, MemTxAttrs attrs,
                             uint8_t *buf, int len, bool is_write)
{
    hwaddr l;
    uint8_t *ptr;
    uint64_t val;
    hwaddr addr1;
    MemoryRegion *mr;
    MemTxResult result = MEMTX_OK;
    bool release_lock = false;

    rcu_read_lock();
    while (len > 0) {
        l = len;
        mr = address_space_translate(as, addr, &addr1, &l, is_write);

        if (is_write) {
            if (!memory_access_is_direct(mr, is_write)) {
                release_lock |= prepare_mmio_access(mr);
                l = memory_access_size(mr, l, addr1);
                 
                switch (l) {
                case 8:
                     
                    val = ldq_p(buf);
                    result |= memory_region_dispatch_write(mr, addr1, val, 8,
                                                           attrs);
                    break;
                case 4:
                     
                    val = ldl_p(buf);
                    result |= memory_region_dispatch_write(mr, addr1, val, 4,
                                                           attrs);
                    break;
                case 2:
                     
                    val = lduw_p(buf);
                    result |= memory_region_dispatch_write(mr, addr1, val, 2,
                                                           attrs);
                    break;
                case 1:
                     
                    val = ldub_p(buf);
                    result |= memory_region_dispatch_write(mr, addr1, val, 1,
                                                           attrs);
                    break;
                default:
                    abort();
                }
            } else {
                addr1 += memory_region_get_ram_addr(mr);
                 
                ptr = qemu_get_ram_ptr(addr1);
                memcpy(ptr, buf, l);
                invalidate_and_set_dirty(mr, addr1, l);
            }
        } else {
            if (!memory_access_is_direct(mr, is_write)) {
                 
                release_lock |= prepare_mmio_access(mr);
                l = memory_access_size(mr, l, addr1);
                switch (l) {
                case 8:
                     
                    result |= memory_region_dispatch_read(mr, addr1, &val, 8,
                                                          attrs);
                    stq_p(buf, val);
                    break;
                case 4:
                     
                    result |= memory_region_dispatch_read(mr, addr1, &val, 4,
                                                          attrs);
                    stl_p(buf, val);
                    break;
                case 2:
                     
                    result |= memory_region_dispatch_read(mr, addr1, &val, 2,
                                                          attrs);
                    stw_p(buf, val);
                    break;
                case 1:
                     
                    result |= memory_region_dispatch_read(mr, addr1, &val, 1,
                                                          attrs);
                    stb_p(buf, val);
                    break;
                default:
                    abort();
                }
            } else {
                 
                ptr = qemu_get_ram_ptr(mr->ram_addr + addr1);
                memcpy(buf, ptr, l);
            }
        }

        if (release_lock) {
            qemu_mutex_unlock_iothread();
            release_lock = false;
        }

        len -= l;
        buf += l;
        addr += l;
    }
    rcu_read_unlock();

    return result;
}
