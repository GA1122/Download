e1000e_core_pci_uninit(E1000ECore *core)
{
    int i;

    timer_del(core->autoneg_timer);
    timer_free(core->autoneg_timer);

    e1000e_intrmgr_pci_unint(core);

    qemu_del_vm_change_state_handler(core->vmstate);

    for (i = 0; i < E1000E_NUM_QUEUES; i++) {
        net_tx_pkt_reset(core->tx[i].tx_pkt);
        net_tx_pkt_uninit(core->tx[i].tx_pkt);
    }

    net_rx_pkt_uninit(core->rx_pkt);
}
