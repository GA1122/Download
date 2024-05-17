static void e1000e_update_flowctl_status(E1000ECore *core)
{
    if (e1000e_have_autoneg(core) &&
        core->phy[0][PHY_STATUS] & MII_SR_AUTONEG_COMPLETE) {
        trace_e1000e_link_autoneg_flowctl(true);
        core->mac[CTRL] |= E1000_CTRL_TFCE | E1000_CTRL_RFCE;
    } else {
        trace_e1000e_link_autoneg_flowctl(false);
    }
}
