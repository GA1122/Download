e1000e_have_autoneg(E1000ECore *core)
{
    return core->phy[0][PHY_CTRL] & MII_CR_AUTO_NEG_EN;
}
