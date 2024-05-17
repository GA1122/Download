e1000e_set_ims(E1000ECore *core, int index, uint32_t val)
{
    static const uint32_t ims_ext_mask =
        E1000_IMS_RXQ0 | E1000_IMS_RXQ1 |
        E1000_IMS_TXQ0 | E1000_IMS_TXQ1 |
        E1000_IMS_OTHER;

    static const uint32_t ims_valid_mask =
        E1000_IMS_TXDW      | E1000_IMS_TXQE    | E1000_IMS_LSC  |
        E1000_IMS_RXDMT0    | E1000_IMS_RXO     | E1000_IMS_RXT0 |
        E1000_IMS_MDAC      | E1000_IMS_TXD_LOW | E1000_IMS_SRPD |
        E1000_IMS_ACK       | E1000_IMS_MNG     | E1000_IMS_RXQ0 |
        E1000_IMS_RXQ1      | E1000_IMS_TXQ0    | E1000_IMS_TXQ1 |
        E1000_IMS_OTHER;

    uint32_t valid_val = val & ims_valid_mask;

    trace_e1000e_irq_set_ims(val, core->mac[IMS], core->mac[IMS] | valid_val);
    core->mac[IMS] |= valid_val;

    if ((valid_val & ims_ext_mask) &&
        (core->mac[CTRL_EXT] & E1000_CTRL_EXT_PBA_CLR) &&
        msix_enabled(core->owner)) {
        e1000e_msix_clear(core, valid_val);
    }

    if ((valid_val == ims_valid_mask) &&
        (core->mac[CTRL_EXT] & E1000_CTRL_EXT_INT_TIMERS_CLEAR_ENA)) {
        trace_e1000e_irq_fire_all_timers(val);
        e1000e_intrmgr_fire_all_timers(core);
    }

    e1000e_update_interrupt_state(core);
}
