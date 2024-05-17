static void nci_rf_deactivate_ntf_packet(struct nci_dev *ndev,
					 struct sk_buff *skb)
{
	struct nci_rf_deactivate_ntf *ntf = (void *) skb->data;

	pr_debug("entry, type 0x%x, reason 0x%x\n", ntf->type, ntf->reason);

	 
	skb_queue_purge(&ndev->tx_q);

	 
	if (ndev->rx_data_reassembly) {
		kfree_skb(ndev->rx_data_reassembly);
		ndev->rx_data_reassembly = NULL;
	}

	 
	if (test_bit(NCI_DATA_EXCHANGE, &ndev->flags))
		nci_data_exchange_complete(ndev, NULL, -EIO);

	nci_clear_target_list(ndev);
	atomic_set(&ndev->state, NCI_IDLE);
	nci_req_complete(ndev, NCI_STATUS_OK);
}