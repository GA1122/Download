void nfc_llcp_mac_is_down(struct nfc_dev *dev)
{
	struct nfc_llcp_local *local;

	local = nfc_llcp_find_local(dev);
	if (local == NULL)
		return;

	local->remote_miu = LLCP_DEFAULT_MIU;
	local->remote_lto = LLCP_DEFAULT_LTO;

	 
	nfc_llcp_socket_release(local, true, 0);
}
