e1000e_write_rx_descr(E1000ECore *core, uint8_t *desc,
struct NetRxPkt *pkt, const E1000E_RSSInfo *rss_info,
    size_t ps_hdr_len, uint16_t(*written)[MAX_PS_BUFFERS])
{
    if (e1000e_rx_use_legacy_descriptor(core)) {
        assert(ps_hdr_len == 0);
        e1000e_write_lgcy_rx_descr(core, desc, pkt, rss_info, (*written)[0]);
    } else {
        if (core->mac[RCTL] & E1000_RCTL_DTYP_PS) {
            e1000e_write_ps_rx_descr(core, desc, pkt, rss_info,
                                      ps_hdr_len, written);
        } else {
            assert(ps_hdr_len == 0);
            e1000e_write_ext_rx_descr(core, desc, pkt, rss_info,
                                       (*written)[0]);
        }
    }
}
