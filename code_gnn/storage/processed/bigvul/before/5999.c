e1000e_link_down(E1000ECore *core)
{
    e1000x_update_regs_on_link_down(core->mac, core->phy[0]);
    e1000e_update_flowctl_status(core);
}
