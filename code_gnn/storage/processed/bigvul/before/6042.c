e1000e_rx_fix_l4_csum(E1000ECore *core, struct NetRxPkt *pkt)
{
    if (net_rx_pkt_has_virt_hdr(pkt)) {
        struct virtio_net_hdr *vhdr = net_rx_pkt_get_vhdr(pkt);

        if (vhdr->flags & VIRTIO_NET_HDR_F_NEEDS_CSUM) {
            net_rx_pkt_fix_l4_csum(pkt);
        }
    }
}
