e1000e_core_pci_realize(E1000ECore     *core,
                        const uint16_t *eeprom_templ,
                        uint32_t        eeprom_size,
                        const uint8_t  *macaddr)
{
    int i;

    core->autoneg_timer = timer_new_ms(QEMU_CLOCK_VIRTUAL,
                                       e1000e_autoneg_timer, core);
    e1000e_intrmgr_pci_realize(core);

    core->vmstate =
        qemu_add_vm_change_state_handler(e1000e_vm_state_change, core);

    for (i = 0; i < E1000E_NUM_QUEUES; i++) {
        net_tx_pkt_init(&core->tx[i].tx_pkt, core->owner,
                        E1000E_MAX_TX_FRAGS, core->has_vnet);
    }

    net_rx_pkt_init(&core->rx_pkt, core->has_vnet);

    e1000x_core_prepare_eeprom(core->eeprom,
                               eeprom_templ,
                               eeprom_size,
                               PCI_DEVICE_GET_CLASS(core->owner)->device_id,
                               macaddr);
    e1000e_update_rx_offloads(core);
}
