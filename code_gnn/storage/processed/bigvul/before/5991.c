e1000e_intrmgr_pci_unint(E1000ECore *core)
{
    int i;

    timer_del(core->radv.timer);
    timer_free(core->radv.timer);
    timer_del(core->rdtr.timer);
    timer_free(core->rdtr.timer);
    timer_del(core->raid.timer);
    timer_free(core->raid.timer);

    timer_del(core->tadv.timer);
    timer_free(core->tadv.timer);
    timer_del(core->tidv.timer);
    timer_free(core->tidv.timer);

    timer_del(core->itr.timer);
    timer_free(core->itr.timer);

    for (i = 0; i < E1000E_MSIX_VEC_NUM; i++) {
        timer_del(core->eitr[i].timer);
        timer_free(core->eitr[i].timer);
    }
}
