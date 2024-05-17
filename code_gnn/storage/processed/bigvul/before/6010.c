e1000e_mac_writereg(E1000ECore *core, int index, uint32_t val)
{
    core->mac[index] = val;
}
