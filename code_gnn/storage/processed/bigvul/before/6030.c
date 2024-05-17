e1000e_receive_filter(E1000ECore *core, const uint8_t *buf, int size)
{
    uint32_t rctl = core->mac[RCTL];

    if (e1000x_is_vlan_packet(buf, core->vet) &&
        e1000x_vlan_rx_filter_enabled(core->mac)) {
        uint16_t vid = lduw_be_p(buf + 14);
        uint32_t vfta = ldl_le_p((uint32_t *)(core->mac + VFTA) +
                                 ((vid >> 5) & 0x7f));
        if ((vfta & (1 << (vid & 0x1f))) == 0) {
            trace_e1000e_rx_flt_vlan_mismatch(vid);
            return false;
        } else {
            trace_e1000e_rx_flt_vlan_match(vid);
        }
    }

    switch (net_rx_pkt_get_packet_type(core->rx_pkt)) {
    case ETH_PKT_UCAST:
        if (rctl & E1000_RCTL_UPE) {
            return true;  
        }
        break;

    case ETH_PKT_BCAST:
        if (rctl & E1000_RCTL_BAM) {
            return true;  
        }
        break;

    case ETH_PKT_MCAST:
        if (rctl & E1000_RCTL_MPE) {
            return true;  
        }
        break;

    default:
        g_assert_not_reached();
    }

    return e1000x_rx_group_filter(core->mac, buf);
}
