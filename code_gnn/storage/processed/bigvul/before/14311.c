void address_space_stw(AddressSpace *as, hwaddr addr, uint32_t val,
                       MemTxAttrs attrs, MemTxResult *result)
{
    address_space_stw_internal(as, addr, val, attrs, result,
                               DEVICE_NATIVE_ENDIAN);
}