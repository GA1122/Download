e1000e_core_set_link_status(E1000ECore *core)
{
    NetClientState *nc = qemu_get_queue(core->owner_nic);
    uint32_t old_status = core->mac[STATUS];

    trace_e1000e_link_status_changed(nc->link_down ? false : true);

    if (nc->link_down) {
        e1000x_update_regs_on_link_down(core->mac, core->phy[0]);
    } else {
        if (e1000e_have_autoneg(core) &&
            !(core->phy[0][PHY_STATUS] & MII_SR_AUTONEG_COMPLETE)) {
            e1000x_restart_autoneg(core->mac, core->phy[0],
                                   core->autoneg_timer);
        } else {
            e1000x_update_regs_on_link_up(core->mac, core->phy[0]);
            e1000e_start_recv(core);
        }
    }

    if (core->mac[STATUS] != old_status) {
        e1000e_set_interrupt_cause(core, E1000_ICR_LSC);
    }
}
