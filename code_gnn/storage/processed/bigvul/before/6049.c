e1000e_set_12bit(E1000ECore *core, int index, uint32_t val)
{
    core->mac[index] = val & 0xfff;
}
