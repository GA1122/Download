e1000e_lower_legacy_irq(E1000ECore *core)
{
    trace_e1000e_irq_legacy_notify(false);
    pci_set_irq(core->owner, 0);
}
