e1000e_set_tdt(E1000ECore *core, int index, uint32_t val)
{
    E1000E_TxRing txr;
    int qidx = e1000e_mq_queue_idx(TDT, index);
    uint32_t tarc_reg = (qidx == 0) ? TARC0 : TARC1;

    core->mac[index] = val & 0xffff;

    if (core->mac[tarc_reg] & E1000_TARC_ENABLE) {
        e1000e_tx_ring_init(core, &txr, qidx);
        e1000e_start_xmit(core, &txr);
    }
}
