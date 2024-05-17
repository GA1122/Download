e1000e_tx_pkt_send(E1000ECore *core, struct e1000e_tx *tx, int queue_index)
{
    int target_queue = MIN(core->max_queue_num, queue_index);
    NetClientState *queue = qemu_get_subqueue(core->owner_nic, target_queue);

    e1000e_setup_tx_offloads(core, tx);

    net_tx_pkt_dump(tx->tx_pkt);

    if ((core->phy[0][PHY_CTRL] & MII_CR_LOOPBACK) ||
        ((core->mac[RCTL] & E1000_RCTL_LBM_MAC) == E1000_RCTL_LBM_MAC)) {
        return net_tx_pkt_send_loopback(tx->tx_pkt, queue);
    } else {
        return net_tx_pkt_send(tx->tx_pkt, queue);
    }
}
