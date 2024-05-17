e1000e_update_rx_stats(E1000ECore *core,
                       size_t data_size,
                       size_t data_fcs_size)
{
    e1000x_update_rx_total_stats(core->mac, data_size, data_fcs_size);

    switch (net_rx_pkt_get_packet_type(core->rx_pkt)) {
    case ETH_PKT_BCAST:
        e1000x_inc_reg_if_not_full(core->mac, BPRC);
        break;

    case ETH_PKT_MCAST:
        e1000x_inc_reg_if_not_full(core->mac, MPRC);
        break;

    default:
        break;
    }
}
