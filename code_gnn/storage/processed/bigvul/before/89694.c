u8 nfc_llcp_get_sdp_ssap(struct nfc_llcp_local *local,
			 struct nfc_llcp_sock *sock)
{
	mutex_lock(&local->sdp_lock);

	if (sock->service_name != NULL && sock->service_name_len > 0) {
		int ssap = nfc_llcp_wks_sap(sock->service_name,
					    sock->service_name_len);

		if (ssap > 0) {
			pr_debug("WKS %d\n", ssap);

			 
			if (local->local_wks & BIT(ssap)) {
				mutex_unlock(&local->sdp_lock);

				return LLCP_SAP_MAX;
			}

			set_bit(ssap, &local->local_wks);
			mutex_unlock(&local->sdp_lock);

			return ssap;
		}

		 
		if (nfc_llcp_sock_from_sn(local, sock->service_name,
					  sock->service_name_len) != NULL) {
			mutex_unlock(&local->sdp_lock);

			return LLCP_SAP_MAX;
		}

		mutex_unlock(&local->sdp_lock);

		return LLCP_SDP_UNBOUND;

	} else if (sock->ssap != 0 && sock->ssap < LLCP_WKS_NUM_SAP) {
		if (!test_bit(sock->ssap, &local->local_wks)) {
			set_bit(sock->ssap, &local->local_wks);
			mutex_unlock(&local->sdp_lock);

			return sock->ssap;
		}
	}

	mutex_unlock(&local->sdp_lock);

	return LLCP_SAP_MAX;
}
