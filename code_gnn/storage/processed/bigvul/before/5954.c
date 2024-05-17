e1000e_autoneg_pause(E1000ECore *core)
{
    timer_del(core->autoneg_timer);
}
