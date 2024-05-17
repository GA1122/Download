static int pvc_open(struct net_device *dev)
{
	pvc_device *pvc = dev->ml_priv;

	if ((pvc->frad->flags & IFF_UP) == 0)
		return -EIO;   

	if (pvc->open_count++ == 0) {
		hdlc_device *hdlc = dev_to_hdlc(pvc->frad);
		if (state(hdlc)->settings.lmi == LMI_NONE)
			pvc->state.active = netif_carrier_ok(pvc->frad);

		pvc_carrier(pvc->state.active, pvc);
		state(hdlc)->dce_changed = 1;
	}
	return 0;
}