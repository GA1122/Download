e1000e_mac_swsm_read(E1000ECore *core, int index)
{
    uint32_t val = core->mac[SWSM];
    core->mac[SWSM] = val | 1;
    return val;
}
