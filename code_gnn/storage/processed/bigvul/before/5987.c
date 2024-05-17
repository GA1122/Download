e1000e_intrmgr_on_throttling_timer(void *opaque)
{
    E1000IntrDelayTimer *timer = opaque;

    assert(!msix_enabled(timer->core->owner));

    timer->running = false;

    if (!timer->core->itr_intr_pending) {
        trace_e1000e_irq_throttling_no_pending_interrupts();
        return;
    }

    if (msi_enabled(timer->core->owner)) {
        trace_e1000e_irq_msi_notify_postponed();
        e1000e_set_interrupt_cause(timer->core, 0);
    } else {
        trace_e1000e_irq_legacy_notify_postponed();
        e1000e_set_interrupt_cause(timer->core, 0);
    }
}
