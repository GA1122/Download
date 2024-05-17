e1000e_phy_reg_write(E1000ECore *core, uint8_t page,
                     uint32_t addr, uint16_t data)
{
    assert(page < E1000E_PHY_PAGES);
    assert(addr < E1000E_PHY_PAGE_SIZE);

    if (e1000e_phyreg_writeops[page][addr]) {
        e1000e_phyreg_writeops[page][addr](core, addr, data);
    } else {
        core->phy[page][addr] = data;
    }
}
