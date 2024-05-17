e1000e_set_rxdctl(E1000ECore *core, int index, uint32_t val)
{
    core->mac[RXDCTL] = core->mac[RXDCTL1] = val;
}
