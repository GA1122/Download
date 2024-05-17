e1000e_core_read(E1000ECore *core, hwaddr addr, unsigned size)
{
    uint64_t val;
    uint16_t index = e1000e_get_reg_index_with_offset(mac_reg_access, addr);

    if (index < E1000E_NREADOPS && e1000e_macreg_readops[index]) {
        if (mac_reg_access[index] & MAC_ACCESS_PARTIAL) {
            trace_e1000e_wrn_regs_read_trivial(index << 2);
        }
        val = e1000e_macreg_readops[index](core, index);
        trace_e1000e_core_read(index << 2, size, val);
        return val;
    } else {
        trace_e1000e_wrn_regs_read_unknown(index << 2, size);
    }
    return 0;
}
