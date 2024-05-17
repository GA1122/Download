e1000e_autoneg_resume(E1000ECore *core)
{
    if (e1000e_have_autoneg(core) &&
        !(core->phy[0][PHY_STATUS] & MII_SR_AUTONEG_COMPLETE)) {
        qemu_get_queue(core->owner_nic)->link_down = false;
        timer_mod(core->autoneg_timer,
                  qemu_clock_get_ms(QEMU_CLOCK_VIRTUAL) + 500);
    }
}
