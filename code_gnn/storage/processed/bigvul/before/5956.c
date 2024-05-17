e1000e_autoneg_timer(void *opaque)
{
    E1000ECore *core = opaque;
    if (!qemu_get_queue(core->owner_nic)->link_down) {
        e1000x_update_regs_on_autoneg_done(core->mac, core->phy[0]);
        e1000e_start_recv(core);

        e1000e_update_flowctl_status(core);
         
        e1000e_set_interrupt_cause(core, E1000_ICR_LSC);
    }
}
