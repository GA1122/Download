e1000e_rx_descr_threshold_hit(E1000ECore *core, const E1000E_RingInfo *rxi)
{
    return e1000e_ring_free_descr_num(core, rxi) ==
           e1000e_ring_len(core, rxi) >> core->rxbuf_min_shift;
}
