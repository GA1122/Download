e1000e_intrmgr_on_msix_throttling_timer(void *opaque)
{
    E1000IntrDelayTimer *timer = opaque;
    int idx = timer - &timer->core->eitr[0];

    assert(msix_enabled(timer->core->owner));

    timer->running = false;

    if (!timer->core->eitr_intr_pending[idx]) {
        trace_e1000e_irq_throttling_no_pending_vec(idx);
        return;
    }

    trace_e1000e_irq_msix_notify_postponed_vec(idx);
    msix_notify(timer->core->owner, idx);
}
