e1000e_set_fcrtl(E1000ECore *core, int index, uint32_t val)
{
    core->mac[FCRTL] = val & 0x8000FFF8;
}
