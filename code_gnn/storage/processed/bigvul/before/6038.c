e1000e_rss_enabled(E1000ECore *core)
{
    return E1000_MRQC_ENABLED(core->mac[MRQC]) &&
           !e1000e_rx_csum_enabled(core) &&
           !e1000e_rx_use_legacy_descriptor(core);
}
