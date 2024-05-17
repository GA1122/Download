e1000e_send_msi(E1000ECore *core, bool msix)
{
    uint32_t causes = core->mac[ICR] & core->mac[IMS] & ~E1000_ICR_ASSERTED;

    if (msix) {
        e1000e_msix_notify(core, causes);
    } else {
        if (!e1000e_itr_should_postpone(core)) {
            trace_e1000e_irq_msi_notify(causes);
            msi_notify(core->owner, 0);
        }
    }
}
