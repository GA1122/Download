static uint64_t ahci_idp_read(void *opaque, hwaddr addr,
                              unsigned size)
{
    AHCIState *s = opaque;

    if (addr == s->idp_offset) {
         
        return s->idp_index;
    } else if (addr == s->idp_offset + 4) {
         
        return ahci_mem_read(opaque, s->idp_index, size);
    } else {
        return 0;
    }
}
