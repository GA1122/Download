e1000e_core_write(E1000ECore *core, hwaddr addr, uint64_t val, unsigned size)
{
    uint16_t index = e1000e_get_reg_index_with_offset(mac_reg_access, addr);

    if (index < E1000E_NWRITEOPS && e1000e_macreg_writeops[index]) {
        if (mac_reg_access[index] & MAC_ACCESS_PARTIAL) {
            trace_e1000e_wrn_regs_write_trivial(index << 2);
        }
        trace_e1000e_core_write(index << 2, size, val);
        e1000e_macreg_writeops[index](core, index, val);
    } else if (index < E1000E_NREADOPS && e1000e_macreg_readops[index]) {
        trace_e1000e_wrn_regs_write_ro(index << 2, size, val);
    } else {
        trace_e1000e_wrn_regs_write_unknown(index << 2, size, val);
    }
}
