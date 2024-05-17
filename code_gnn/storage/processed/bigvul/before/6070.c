e1000e_set_phy_ctrl(E1000ECore *core, int index, uint16_t val)
{
     
    core->phy[0][PHY_CTRL] = val & ~(0x3f |
                                     MII_CR_RESET |
                                     MII_CR_RESTART_AUTO_NEG);

    if ((val & MII_CR_RESTART_AUTO_NEG) &&
        e1000e_have_autoneg(core)) {
        e1000x_restart_autoneg(core->mac, core->phy[0], core->autoneg_timer);
    }
}
