e1000e_intrmgr_reset(E1000ECore *core)
{
    int i;

    core->delayed_causes = 0;

    e1000e_intrmgr_stop_delay_timers(core);

    e1000e_intrmgr_stop_timer(&core->itr);

    for (i = 0; i < E1000E_MSIX_VEC_NUM; i++) {
        e1000e_intrmgr_stop_timer(&core->eitr[i]);
    }
}
