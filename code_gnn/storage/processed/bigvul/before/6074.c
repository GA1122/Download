e1000e_set_rdt(E1000ECore *core, int index, uint32_t val)
{
    core->mac[index] = val & 0xffff;
    trace_e1000e_rx_set_rdt(e1000e_mq_queue_idx(RDT0, index), val);
    e1000e_start_recv(core);
}
