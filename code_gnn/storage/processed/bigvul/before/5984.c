e1000e_intrmgr_fire_delayed_interrupts(E1000ECore *core)
{
    trace_e1000e_irq_fire_delayed_interrupts();
    e1000e_set_interrupt_cause(core, 0);
}
