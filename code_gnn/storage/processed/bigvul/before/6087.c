e1000e_start_xmit(E1000ECore *core, const E1000E_TxRing *txr)
{
    dma_addr_t base;
    struct e1000_tx_desc desc;
    bool ide = false;
    const E1000E_RingInfo *txi = txr->i;
    uint32_t cause = E1000_ICS_TXQE;

    if (!(core->mac[TCTL] & E1000_TCTL_EN)) {
        trace_e1000e_tx_disabled();
        return;
    }

    while (!e1000e_ring_empty(core, txi)) {
        base = e1000e_ring_head_descr(core, txi);

        pci_dma_read(core->owner, base, &desc, sizeof(desc));

        trace_e1000e_tx_descr((void *)(intptr_t)desc.buffer_addr,
                              desc.lower.data, desc.upper.data);

        e1000e_process_tx_desc(core, txr->tx, &desc, txi->idx);
        cause |= e1000e_txdesc_writeback(core, base, &desc, &ide, txi->idx);

        e1000e_ring_advance(core, txi, 1);
    }

    if (!ide || !e1000e_intrmgr_delay_tx_causes(core, &cause)) {
        e1000e_set_interrupt_cause(core, cause);
    }
}
