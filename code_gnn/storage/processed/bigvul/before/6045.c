e1000e_rx_use_legacy_descriptor(E1000ECore *core)
{
    return (core->mac[RFCTL] & E1000_RFCTL_EXTEN) ? false : true;
}
