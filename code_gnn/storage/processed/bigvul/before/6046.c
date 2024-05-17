e1000e_rx_use_ps_descriptor(E1000ECore *core)
{
    return !e1000e_rx_use_legacy_descriptor(core) &&
           (core->mac[RCTL] & E1000_RCTL_DTYP_PS);
}
