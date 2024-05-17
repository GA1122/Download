bool address_space_read(AddressSpace *as, hwaddr addr, uint8_t *buf, int len)
{
    return address_space_rw(as, addr, buf, len, false);
}
