e1000e_set_tctl(E1000ECore *core, int index, uint32_t val)
{
    E1000E_TxRing txr;
    core->mac[index] = val;

    if (core->mac[TARC0] & E1000_TARC_ENABLE) {
        e1000e_tx_ring_init(core, &txr, 0);
        e1000e_start_xmit(core, &txr);
    }

    if (core->mac[TARC1] & E1000_TARC_ENABLE) {
        e1000e_tx_ring_init(core, &txr, 1);
        e1000e_start_xmit(core, &txr);
    }
}
