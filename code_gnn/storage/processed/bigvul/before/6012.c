e1000e_msix_clear(E1000ECore *core, uint32_t causes)
{
    if (causes & E1000_ICR_RXQ0) {
        e1000e_msix_clear_one(core, E1000_ICR_RXQ0,
                              E1000_IVAR_RXQ0(core->mac[IVAR]));
    }

    if (causes & E1000_ICR_RXQ1) {
        e1000e_msix_clear_one(core, E1000_ICR_RXQ1,
                              E1000_IVAR_RXQ1(core->mac[IVAR]));
    }

    if (causes & E1000_ICR_TXQ0) {
        e1000e_msix_clear_one(core, E1000_ICR_TXQ0,
                              E1000_IVAR_TXQ0(core->mac[IVAR]));
    }

    if (causes & E1000_ICR_TXQ1) {
        e1000e_msix_clear_one(core, E1000_ICR_TXQ1,
                              E1000_IVAR_TXQ1(core->mac[IVAR]));
    }

    if (causes & E1000_ICR_OTHER) {
        e1000e_msix_clear_one(core, E1000_ICR_OTHER,
                              E1000_IVAR_OTHER(core->mac[IVAR]));
    }
}
