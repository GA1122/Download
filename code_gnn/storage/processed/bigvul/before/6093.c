e1000e_update_interrupt_state(E1000ECore *core)
{
    bool interrupts_pending;
    bool is_msix = msix_enabled(core->owner);

     
    if (is_msix) {
        if (core->mac[ICR] & E1000_ICR_OTHER_CAUSES) {
            core->mac[ICR] |= E1000_ICR_OTHER;
            trace_e1000e_irq_add_msi_other(core->mac[ICR]);
        }
    }

    e1000e_fix_icr_asserted(core);

     
    core->mac[ICS] = core->mac[ICR];

    interrupts_pending = (core->mac[IMS] & core->mac[ICR]) ? true : false;

    trace_e1000e_irq_pending_interrupts(core->mac[ICR] & core->mac[IMS],
                                        core->mac[ICR], core->mac[IMS]);

    if (is_msix || msi_enabled(core->owner)) {
        if (interrupts_pending) {
            e1000e_send_msi(core, is_msix);
        }
    } else {
        if (interrupts_pending) {
            if (!e1000e_itr_should_postpone(core)) {
                e1000e_raise_legacy_irq(core);
            }
        } else {
            e1000e_lower_legacy_irq(core);
        }
    }
}
