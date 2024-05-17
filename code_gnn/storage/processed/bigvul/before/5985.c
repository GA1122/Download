e1000e_intrmgr_initialize_all_timers(E1000ECore *core, bool create)
{
    int i;

    core->radv.delay_reg = RADV;
    core->rdtr.delay_reg = RDTR;
    core->raid.delay_reg = RAID;
    core->tadv.delay_reg = TADV;
    core->tidv.delay_reg = TIDV;

    core->radv.delay_resolution_ns = E1000_INTR_DELAY_NS_RES;
    core->rdtr.delay_resolution_ns = E1000_INTR_DELAY_NS_RES;
    core->raid.delay_resolution_ns = E1000_INTR_DELAY_NS_RES;
    core->tadv.delay_resolution_ns = E1000_INTR_DELAY_NS_RES;
    core->tidv.delay_resolution_ns = E1000_INTR_DELAY_NS_RES;

    core->radv.core = core;
    core->rdtr.core = core;
    core->raid.core = core;
    core->tadv.core = core;
    core->tidv.core = core;

    core->itr.core = core;
    core->itr.delay_reg = ITR;
    core->itr.delay_resolution_ns = E1000_INTR_THROTTLING_NS_RES;

    for (i = 0; i < E1000E_MSIX_VEC_NUM; i++) {
        core->eitr[i].core = core;
        core->eitr[i].delay_reg = EITR + i;
        core->eitr[i].delay_resolution_ns = E1000_INTR_THROTTLING_NS_RES;
    }

    if (!create) {
        return;
    }

    core->radv.timer =
        timer_new_ns(QEMU_CLOCK_VIRTUAL, e1000e_intrmgr_on_timer, &core->radv);
    core->rdtr.timer =
        timer_new_ns(QEMU_CLOCK_VIRTUAL, e1000e_intrmgr_on_timer, &core->rdtr);
    core->raid.timer =
        timer_new_ns(QEMU_CLOCK_VIRTUAL, e1000e_intrmgr_on_timer, &core->raid);

    core->tadv.timer =
        timer_new_ns(QEMU_CLOCK_VIRTUAL, e1000e_intrmgr_on_timer, &core->tadv);
    core->tidv.timer =
        timer_new_ns(QEMU_CLOCK_VIRTUAL, e1000e_intrmgr_on_timer, &core->tidv);

    core->itr.timer = timer_new_ns(QEMU_CLOCK_VIRTUAL,
                                   e1000e_intrmgr_on_throttling_timer,
                                   &core->itr);

    for (i = 0; i < E1000E_MSIX_VEC_NUM; i++) {
        core->eitr[i].timer =
            timer_new_ns(QEMU_CLOCK_VIRTUAL,
                         e1000e_intrmgr_on_msix_throttling_timer,
                         &core->eitr[i]);
    }
}
