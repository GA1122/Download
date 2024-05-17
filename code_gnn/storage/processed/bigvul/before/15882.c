static uint64_t msix_pba_mmio_read(void *opaque, hwaddr addr,
                                   unsigned size)
{
    PCIDevice *dev = opaque;
    if (dev->msix_vector_poll_notifier) {
        unsigned vector_start = addr * 8;
        unsigned vector_end = MIN(addr + size * 8, dev->msix_entries_nr);
        dev->msix_vector_poll_notifier(dev, vector_start, vector_end);
    }

     return pci_get_long(dev->msix_pba + addr);
 }
