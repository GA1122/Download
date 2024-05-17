static void nci_rf_intf_activated_ntf_packet(struct nci_dev *ndev,
					     struct sk_buff *skb)
{
	struct nci_rf_intf_activated_ntf ntf;
	__u8 *data = skb->data;
	int err = NCI_STATUS_OK;

	ntf.rf_discovery_id = *data++;
	ntf.rf_interface = *data++;
	ntf.rf_protocol = *data++;
	ntf.activation_rf_tech_and_mode = *data++;
	ntf.max_data_pkt_payload_size = *data++;
	ntf.initial_num_credits = *data++;
	ntf.rf_tech_specific_params_len = *data++;

	pr_debug("rf_discovery_id %d\n", ntf.rf_discovery_id);
	pr_debug("rf_interface 0x%x\n", ntf.rf_interface);
	pr_debug("rf_protocol 0x%x\n", ntf.rf_protocol);
	pr_debug("activation_rf_tech_and_mode 0x%x\n",
		 ntf.activation_rf_tech_and_mode);
	pr_debug("max_data_pkt_payload_size 0x%x\n",
		 ntf.max_data_pkt_payload_size);
	pr_debug("initial_num_credits 0x%x\n",
		 ntf.initial_num_credits);
	pr_debug("rf_tech_specific_params_len %d\n",
		 ntf.rf_tech_specific_params_len);

	if (ntf.rf_tech_specific_params_len > 0) {
		switch (ntf.activation_rf_tech_and_mode) {
		case NCI_NFC_A_PASSIVE_POLL_MODE:
			data = nci_extract_rf_params_nfca_passive_poll(ndev,
				&(ntf.rf_tech_specific_params.nfca_poll), data);
			break;

		case NCI_NFC_B_PASSIVE_POLL_MODE:
			data = nci_extract_rf_params_nfcb_passive_poll(ndev,
				&(ntf.rf_tech_specific_params.nfcb_poll), data);
			break;

		case NCI_NFC_F_PASSIVE_POLL_MODE:
			data = nci_extract_rf_params_nfcf_passive_poll(ndev,
				&(ntf.rf_tech_specific_params.nfcf_poll), data);
			break;

		default:
			pr_err("unsupported activation_rf_tech_and_mode 0x%x\n",
			       ntf.activation_rf_tech_and_mode);
			err = NCI_STATUS_RF_PROTOCOL_ERROR;
			goto exit;
		}
	}

	ntf.data_exch_rf_tech_and_mode = *data++;
	ntf.data_exch_tx_bit_rate = *data++;
	ntf.data_exch_rx_bit_rate = *data++;
	ntf.activation_params_len = *data++;

	pr_debug("data_exch_rf_tech_and_mode 0x%x\n",
		 ntf.data_exch_rf_tech_and_mode);
	pr_debug("data_exch_tx_bit_rate 0x%x\n", ntf.data_exch_tx_bit_rate);
	pr_debug("data_exch_rx_bit_rate 0x%x\n", ntf.data_exch_rx_bit_rate);
	pr_debug("activation_params_len %d\n", ntf.activation_params_len);

	if (ntf.activation_params_len > 0) {
		switch (ntf.rf_interface) {
		case NCI_RF_INTERFACE_ISO_DEP:
			err = nci_extract_activation_params_iso_dep(ndev,
								    &ntf, data);
			break;

		case NCI_RF_INTERFACE_FRAME:
			 
			break;

		default:
			pr_err("unsupported rf_interface 0x%x\n",
			       ntf.rf_interface);
			err = NCI_STATUS_RF_PROTOCOL_ERROR;
			break;
		}
	}

exit:
	if (err == NCI_STATUS_OK) {
		ndev->max_data_pkt_payload_size = ntf.max_data_pkt_payload_size;
		ndev->initial_num_credits = ntf.initial_num_credits;

		 
		atomic_set(&ndev->credits_cnt, ndev->initial_num_credits);
	}

	if (atomic_read(&ndev->state) == NCI_DISCOVERY) {
		 
		atomic_set(&ndev->state, NCI_POLL_ACTIVE);
		if (err == NCI_STATUS_OK)
			nci_target_auto_activated(ndev, &ntf);
	} else {	 
		 
		atomic_set(&ndev->state, NCI_POLL_ACTIVE);
		nci_req_complete(ndev, err);
	}
}