e1000e_set_rxcsum(E1000ECore *core, int index, uint32_t val)
{
    core->mac[RXCSUM] = val;
    e1000e_update_rx_offloads(core);
}
