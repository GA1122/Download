e1000e_tx_wb_interrupt_cause(E1000ECore *core, int queue_idx)
{
    if (!msix_enabled(core->owner)) {
        return E1000_ICR_TXDW;
    }

    return (queue_idx == 0) ? E1000_ICR_TXQ0 : E1000_ICR_TXQ1;
}
