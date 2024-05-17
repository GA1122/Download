static int efx_probe_port(struct efx_nic *efx)
{
	unsigned char *perm_addr;
	int rc;

	netif_dbg(efx, probe, efx->net_dev, "create port\n");

	if (phy_flash_cfg)
		efx->phy_mode = PHY_MODE_SPECIAL;

	 
	rc = efx->type->probe_port(efx);
	if (rc)
		return rc;

	 
	perm_addr = efx->net_dev->perm_addr;
	if (is_valid_ether_addr(perm_addr)) {
		memcpy(efx->net_dev->dev_addr, perm_addr, ETH_ALEN);
	} else {
		netif_err(efx, probe, efx->net_dev, "invalid MAC address %pM\n",
			  perm_addr);
		if (!allow_bad_hwaddr) {
			rc = -EINVAL;
			goto err;
		}
		random_ether_addr(efx->net_dev->dev_addr);
		netif_info(efx, probe, efx->net_dev,
			   "using locally-generated MAC %pM\n",
			   efx->net_dev->dev_addr);
	}

	return 0;

 err:
	efx->type->remove_port(efx);
	return rc;
}
