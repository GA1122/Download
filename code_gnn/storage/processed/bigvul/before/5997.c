e1000e_is_tcp_ack(E1000ECore *core, struct NetRxPkt *rx_pkt)
{
    if (!net_rx_pkt_is_tcp_ack(rx_pkt)) {
        return false;
    }

    if (core->mac[RFCTL] & E1000_RFCTL_ACK_DATA_DIS) {
        return !net_rx_pkt_has_tcp_data(rx_pkt);
    }

    return true;
}
