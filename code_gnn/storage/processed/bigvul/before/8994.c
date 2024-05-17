vmxnet3_get_next_body_rx_descr(VMXNET3State *s,
                               struct Vmxnet3_RxDesc *d,
                               uint32_t *didx,
                               uint32_t *ridx)
{
    vmxnet3_read_next_rx_descr(s, RXQ_IDX, RX_HEAD_BODY_RING, d, didx);

     
    if (d->gen == vmxnet3_get_rx_ring_gen(s, RXQ_IDX, RX_HEAD_BODY_RING)) {
         
        smp_rmb();
         
        vmxnet3_read_next_rx_descr(s, RXQ_IDX, RX_HEAD_BODY_RING, d, didx);
        if (d->btype == VMXNET3_RXD_BTYPE_BODY) {
            vmxnet3_inc_rx_consumption_counter(s, RXQ_IDX, RX_HEAD_BODY_RING);
            *ridx = RX_HEAD_BODY_RING;
            return true;
        }
    }

     
    vmxnet3_read_next_rx_descr(s, RXQ_IDX, RX_BODY_ONLY_RING, d, didx);

     
    if (d->gen == vmxnet3_get_rx_ring_gen(s, RXQ_IDX, RX_BODY_ONLY_RING)) {
         
        smp_rmb();
         
        vmxnet3_read_next_rx_descr(s, RXQ_IDX, RX_BODY_ONLY_RING, d, didx);
        assert(d->btype == VMXNET3_RXD_BTYPE_BODY);
        *ridx = RX_BODY_ONLY_RING;
        vmxnet3_inc_rx_consumption_counter(s, RXQ_IDX, RX_BODY_ONLY_RING);
        return true;
    }

    return false;
}
