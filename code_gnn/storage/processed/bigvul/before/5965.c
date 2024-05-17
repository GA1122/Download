void e1000e_core_pre_save(E1000ECore *core)
{
    int i;
    NetClientState *nc = qemu_get_queue(core->owner_nic);

     
    if (nc->link_down && e1000e_have_autoneg(core)) {
        core->phy[0][PHY_STATUS] |= MII_SR_AUTONEG_COMPLETE;
        e1000e_update_flowctl_status(core);
    }

    for (i = 0; i < ARRAY_SIZE(core->tx); i++) {
        if (net_tx_pkt_has_fragments(core->tx[i].tx_pkt)) {
            core->tx[i].skip_cp = true;
        }
    }
}
