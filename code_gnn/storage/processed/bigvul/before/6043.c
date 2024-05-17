e1000e_rx_l3_cso_enabled(E1000ECore *core)
{
    return !!(core->mac[RXCSUM] & E1000_RXCSUM_IPOFLD);
}
