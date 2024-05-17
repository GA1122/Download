e1000e_has_rxbufs(E1000ECore *core, const E1000E_RingInfo *r,
                  size_t total_size)
{
    uint32_t bufs = e1000e_ring_free_descr_num(core, r);

    trace_e1000e_rx_has_buffers(r->idx, bufs, total_size,
                                core->rx_desc_buf_size);

    return total_size <= bufs / (core->rx_desc_len / E1000_MIN_RX_DESC_LEN) *
                         core->rx_desc_buf_size;
}
