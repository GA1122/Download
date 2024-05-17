e1000e_postpone_interrupt(bool *interrupt_pending,
                           E1000IntrDelayTimer *timer)
{
    if (timer->running) {
        trace_e1000e_irq_postponed_by_xitr(timer->delay_reg << 2);

        *interrupt_pending = true;
        return true;
    }

    if (timer->core->mac[timer->delay_reg] != 0) {
        e1000e_intrmgr_rearm_timer(timer);
    }

    return false;
}
