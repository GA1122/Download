void nci_ntf_packet(struct nci_dev *ndev, struct sk_buff *skb)
{
	__u16 ntf_opcode = nci_opcode(skb->data);

	pr_debug("NCI RX: MT=ntf, PBF=%d, GID=0x%x, OID=0x%x, plen=%d\n",
		 nci_pbf(skb->data),
		 nci_opcode_gid(ntf_opcode),
		 nci_opcode_oid(ntf_opcode),
		 nci_plen(skb->data));

	 
	skb_pull(skb, NCI_CTRL_HDR_SIZE);

	switch (ntf_opcode) {
	case NCI_OP_CORE_CONN_CREDITS_NTF:
		nci_core_conn_credits_ntf_packet(ndev, skb);
		break;

	case NCI_OP_CORE_GENERIC_ERROR_NTF:
		nci_core_generic_error_ntf_packet(ndev, skb);
		break;

	case NCI_OP_CORE_INTF_ERROR_NTF:
		nci_core_conn_intf_error_ntf_packet(ndev, skb);
		break;

	case NCI_OP_RF_DISCOVER_NTF:
		nci_rf_discover_ntf_packet(ndev, skb);
		break;

	case NCI_OP_RF_INTF_ACTIVATED_NTF:
		nci_rf_intf_activated_ntf_packet(ndev, skb);
		break;

	case NCI_OP_RF_DEACTIVATE_NTF:
		nci_rf_deactivate_ntf_packet(ndev, skb);
		break;

	default:
		pr_err("unknown ntf opcode 0x%x\n", ntf_opcode);
		break;
	}

	kfree_skb(skb);
}