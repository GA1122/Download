vmxnet3_get_next_head_rx_descr(VMXNET3State *s,
                               struct Vmxnet3_RxDesc *descr_buf,
                               uint32_t *descr_idx,
                               uint32_t *ridx)
{
    for (;;) {
        uint32_t ring_gen;
        vmxnet3_read_next_rx_descr(s, RXQ_IDX, RX_HEAD_BODY_RING,
                                   descr_buf, descr_idx);

         
        ring_gen = vmxnet3_get_rx_ring_gen(s, RXQ_IDX, RX_HEAD_BODY_RING);
        if (descr_buf->gen != ring_gen) {
            return false;
        }

         
        smp_rmb();
         
        vmxnet3_read_next_rx_descr(s, RXQ_IDX, RX_HEAD_BODY_RING,
                                   descr_buf, descr_idx);

         
        vmxnet3_inc_rx_consumption_counter(s, RXQ_IDX, RX_HEAD_BODY_RING);

         
        if (descr_buf->btype == VMXNET3_RXD_BTYPE_HEAD) {
            *ridx = RX_HEAD_BODY_RING;
            return true;
        }
    }
}
