vmxnet3_pop_next_tx_descr(VMXNET3State *s,
                          int qidx,
                          struct Vmxnet3_TxDesc *txd,
                          uint32_t *descr_idx)
{
    Vmxnet3Ring *ring = &s->txq_descr[qidx].tx_ring;

    vmxnet3_ring_read_curr_cell(ring, txd);
    if (txd->gen == vmxnet3_ring_curr_gen(ring)) {
         
        smp_rmb();
         
        vmxnet3_ring_read_curr_cell(ring, txd);
        VMXNET3_RING_DUMP(VMW_RIPRN, "TX", qidx, ring);
        *descr_idx = vmxnet3_ring_curr_cell_idx(ring);
        vmxnet3_inc_tx_consumption_counter(s, qidx);
        return true;
    }

    return false;
}