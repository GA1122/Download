e1000e_receive_iov(E1000ECore *core, const struct iovec *iov, int iovcnt)
{
    static const int maximum_ethernet_hdr_len = (14 + 4);
     
    static const int min_buf_size = 60;

    uint32_t n = 0;
    uint8_t min_buf[min_buf_size];
    struct iovec min_iov;
    uint8_t *filter_buf;
    size_t size, orig_size;
    size_t iov_ofs = 0;
    E1000E_RxRing rxr;
    E1000E_RSSInfo rss_info;
    size_t total_size;
    ssize_t retval;
    bool rdmts_hit;

    trace_e1000e_rx_receive_iov(iovcnt);

    if (!e1000x_hw_rx_enabled(core->mac)) {
        return -1;
    }

     
    if (core->has_vnet) {
        net_rx_pkt_set_vhdr_iovec(core->rx_pkt, iov, iovcnt);
        iov_ofs = sizeof(struct virtio_net_hdr);
    }

    filter_buf = iov->iov_base + iov_ofs;
    orig_size = iov_size(iov, iovcnt);
    size = orig_size - iov_ofs;

     
    if (size < sizeof(min_buf)) {
        iov_to_buf(iov, iovcnt, iov_ofs, min_buf, size);
        memset(&min_buf[size], 0, sizeof(min_buf) - size);
        e1000x_inc_reg_if_not_full(core->mac, RUC);
        min_iov.iov_base = filter_buf = min_buf;
        min_iov.iov_len = size = sizeof(min_buf);
        iovcnt = 1;
        iov = &min_iov;
        iov_ofs = 0;
    } else if (iov->iov_len < maximum_ethernet_hdr_len) {
         
        iov_to_buf(iov, iovcnt, iov_ofs, min_buf, maximum_ethernet_hdr_len);
        filter_buf = min_buf;
    }

     
    if (e1000x_is_oversized(core->mac, size)) {
        return orig_size;
    }

    net_rx_pkt_set_packet_type(core->rx_pkt,
        get_eth_packet_type(PKT_GET_ETH_HDR(filter_buf)));

    if (!e1000e_receive_filter(core, filter_buf, size)) {
        trace_e1000e_rx_flt_dropped();
        return orig_size;
    }

    net_rx_pkt_attach_iovec_ex(core->rx_pkt, iov, iovcnt, iov_ofs,
                               e1000x_vlan_enabled(core->mac), core->vet);

    e1000e_rss_parse_packet(core, core->rx_pkt, &rss_info);
    e1000e_rx_ring_init(core, &rxr, rss_info.queue);

    trace_e1000e_rx_rss_dispatched_to_queue(rxr.i->idx);

    total_size = net_rx_pkt_get_total_len(core->rx_pkt) +
        e1000x_fcs_len(core->mac);

    if (e1000e_has_rxbufs(core, rxr.i, total_size)) {
        e1000e_rx_fix_l4_csum(core, core->rx_pkt);

        e1000e_write_packet_to_guest(core, core->rx_pkt, &rxr, &rss_info);

        retval = orig_size;

         
        if (total_size < core->mac[RSRPD]) {
            n |= E1000_ICS_SRPD;
        }

         
        if  (!(core->mac[RFCTL] & E1000_RFCTL_ACK_DIS) &&
             (e1000e_is_tcp_ack(core, core->rx_pkt))) {
            n |= E1000_ICS_ACK;
        }

         
        rdmts_hit = e1000e_rx_descr_threshold_hit(core, rxr.i);
        n |= e1000e_rx_wb_interrupt_cause(core, rxr.i->idx, rdmts_hit);

        trace_e1000e_rx_written_to_guest(n);
    } else {
        n |= E1000_ICS_RXO;
        retval = 0;

        trace_e1000e_rx_not_written_to_guest(n);
    }

    if (!e1000e_intrmgr_delay_rx_causes(core, &n)) {
        trace_e1000e_rx_interrupt_set(n);
        e1000e_set_interrupt_cause(core, n);
    } else {
        trace_e1000e_rx_interrupt_delayed(n);
    }

    return retval;
}
