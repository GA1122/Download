e1000e_intrmgr_on_timer(void *opaque)
{
    E1000IntrDelayTimer *timer = opaque;

    trace_e1000e_irq_throttling_timer(timer->delay_reg << 2);

    timer->running = false;
    e1000e_intrmgr_fire_delayed_interrupts(timer->core);
}
