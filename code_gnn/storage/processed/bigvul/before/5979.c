e1000e_intmgr_collect_delayed_causes(E1000ECore *core)
{
    uint32_t res;

    if (msix_enabled(core->owner)) {
        assert(core->delayed_causes == 0);
        return 0;
    }

    res = core->delayed_causes;
    core->delayed_causes = 0;

    e1000e_intrmgr_stop_delay_timers(core);

    return res;
}
