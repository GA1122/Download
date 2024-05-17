e1000e_set_psrctl(E1000ECore *core, int index, uint32_t val)
{
    if ((val & E1000_PSRCTL_BSIZE0_MASK) == 0) {
        hw_error("e1000e: PSRCTL.BSIZE0 cannot be zero");
    }

    if ((val & E1000_PSRCTL_BSIZE1_MASK) == 0) {
        hw_error("e1000e: PSRCTL.BSIZE1 cannot be zero");
    }

    core->mac[PSRCTL] = val;
}
