e1000e_intrmgr_resume(E1000ECore *core)
{
    int i;

    e1000e_intmgr_timer_resume(&core->radv);
    e1000e_intmgr_timer_resume(&core->rdtr);
    e1000e_intmgr_timer_resume(&core->raid);
    e1000e_intmgr_timer_resume(&core->tidv);
    e1000e_intmgr_timer_resume(&core->tadv);

    e1000e_intmgr_timer_resume(&core->itr);

    for (i = 0; i < E1000E_MSIX_VEC_NUM; i++) {
        e1000e_intmgr_timer_resume(&core->eitr[i]);
    }
}
