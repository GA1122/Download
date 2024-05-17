e1000e_intrmgr_stop_delay_timers(E1000ECore *core)
{
    e1000e_intrmgr_stop_timer(&core->radv);
    e1000e_intrmgr_stop_timer(&core->rdtr);
    e1000e_intrmgr_stop_timer(&core->raid);
    e1000e_intrmgr_stop_timer(&core->tidv);
    e1000e_intrmgr_stop_timer(&core->tadv);
}
