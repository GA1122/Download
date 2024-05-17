e1000e_set_fcrth(E1000ECore *core, int index, uint32_t val)
{
    core->mac[FCRTH] = val & 0xFFF8;
}
