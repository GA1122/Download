static void fr_destroy(struct net_device *frad)
{
	hdlc_device *hdlc = dev_to_hdlc(frad);
	pvc_device *pvc = state(hdlc)->first_pvc;
	state(hdlc)->first_pvc = NULL;  
	state(hdlc)->dce_pvc_count = 0;
	state(hdlc)->dce_changed = 1;

	while (pvc) {
		pvc_device *next = pvc->next;
		 
		if (pvc->main)
			unregister_netdevice(pvc->main);

		if (pvc->ether)
			unregister_netdevice(pvc->ether);

		kfree(pvc);
		pvc = next;
	}
}
