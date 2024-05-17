e1000e_intmgr_timer_resume(E1000IntrDelayTimer *timer)
{
    if (timer->running) {
        e1000e_intrmgr_rearm_timer(timer);
    }
}
