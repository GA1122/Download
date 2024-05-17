static void nci_core_conn_credits_ntf_packet(struct nci_dev *ndev,
					     struct sk_buff *skb)
{
	struct nci_core_conn_credit_ntf *ntf = (void *) skb->data;
	int i;

	pr_debug("num_entries %d\n", ntf->num_entries);

	if (ntf->num_entries > NCI_MAX_NUM_CONN)
		ntf->num_entries = NCI_MAX_NUM_CONN;

	 
	for (i = 0; i < ntf->num_entries; i++) {
		ntf->conn_entries[i].conn_id =
			nci_conn_id(&ntf->conn_entries[i].conn_id);

		pr_debug("entry[%d]: conn_id %d, credits %d\n",
			 i, ntf->conn_entries[i].conn_id,
			 ntf->conn_entries[i].credits);

		if (ntf->conn_entries[i].conn_id == NCI_STATIC_RF_CONN_ID) {
			 
			atomic_add(ntf->conn_entries[i].credits,
				   &ndev->credits_cnt);
		}
	}

	 
	if (!skb_queue_empty(&ndev->tx_q))
		queue_work(ndev->tx_wq, &ndev->tx_work);
}