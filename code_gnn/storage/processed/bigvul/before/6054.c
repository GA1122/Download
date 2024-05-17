e1000e_set_dlen(E1000ECore *core, int index, uint32_t val)
{
    core->mac[index] = val & E1000_XDLEN_MASK;
}
