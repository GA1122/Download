e1000e_intrmgr_pause(E1000ECore *core)
{
    int i;

    e1000e_intmgr_timer_pause(&core->radv);
    e1000e_intmgr_timer_pause(&core->rdtr);
    e1000e_intmgr_timer_pause(&core->raid);
    e1000e_intmgr_timer_pause(&core->tidv);
    e1000e_intmgr_timer_pause(&core->tadv);

    e1000e_intmgr_timer_pause(&core->itr);

    for (i = 0; i < E1000E_MSIX_VEC_NUM; i++) {
        e1000e_intmgr_timer_pause(&core->eitr[i]);
    }
}
