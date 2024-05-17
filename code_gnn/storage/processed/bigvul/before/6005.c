e1000e_mac_read_clr4(E1000ECore *core, int index)
{
    uint32_t ret = core->mac[index];

    core->mac[index] = 0;
    return ret;
}
