e1000e_mac_icr_read(E1000ECore *core, int index)
{
    uint32_t ret = core->mac[ICR];
    trace_e1000e_irq_icr_read_entry(ret);

    if (core->mac[IMS] == 0) {
        trace_e1000e_irq_icr_clear_zero_ims();
        core->mac[ICR] = 0;
    }

    if ((core->mac[ICR] & E1000_ICR_ASSERTED) &&
        (core->mac[CTRL_EXT] & E1000_CTRL_EXT_IAME)) {
        trace_e1000e_irq_icr_clear_iame();
        core->mac[ICR] = 0;
        trace_e1000e_irq_icr_process_iame();
        e1000e_clear_ims_bits(core, core->mac[IAM]);
    }

    trace_e1000e_irq_icr_read_exit(core->mac[ICR]);
    e1000e_update_interrupt_state(core);
    return ret;
}
