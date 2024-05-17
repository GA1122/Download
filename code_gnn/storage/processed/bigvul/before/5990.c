e1000e_intrmgr_pci_realize(E1000ECore *core)
{
    e1000e_intrmgr_initialize_all_timers(core, true);
}
