e1000e_phy_reg_check_cap(E1000ECore *core, uint32_t addr,
                         char cap, uint8_t *page)
{
    *page =
        (e1000e_phy_regcap[0][addr] & PHY_ANYPAGE) ? 0
                                                    : core->phy[0][PHY_PAGE];

    if (*page >= E1000E_PHY_PAGES) {
        return false;
    }

    return e1000e_phy_regcap[*page][addr] & cap;
}
