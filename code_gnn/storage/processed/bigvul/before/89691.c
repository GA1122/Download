struct nfc_llcp_local *nfc_llcp_find_local(struct nfc_dev *dev)
{
	struct nfc_llcp_local *local;

	list_for_each_entry(local, &llcp_devices, list)
		if (local->dev == dev)
			return local;

	pr_debug("No device found\n");

	return NULL;
}