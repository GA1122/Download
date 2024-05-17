static void nci_core_conn_intf_error_ntf_packet(struct nci_dev *ndev,
						struct sk_buff *skb)
{
	struct nci_core_intf_error_ntf *ntf = (void *) skb->data;

	ntf->conn_id = nci_conn_id(&ntf->conn_id);

	pr_debug("status 0x%x, conn_id %d\n", ntf->status, ntf->conn_id);

	 
	if (test_bit(NCI_DATA_EXCHANGE, &ndev->flags))
		nci_data_exchange_complete(ndev, NULL, -EIO);
}