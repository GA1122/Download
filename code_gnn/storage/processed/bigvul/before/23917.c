static void fr_set_link_state(int reliable, struct net_device *dev)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
	pvc_device *pvc = state(hdlc)->first_pvc;

	state(hdlc)->reliable = reliable;
	if (reliable) {
		netif_dormant_off(dev);
		state(hdlc)->n391cnt = 0;  
		state(hdlc)->dce_changed = 1;

		if (state(hdlc)->settings.lmi == LMI_NONE) {
			while (pvc) {	 
				pvc_carrier(1, pvc);
				pvc->state.exist = pvc->state.active = 1;
				pvc->state.new = 0;
				pvc = pvc->next;
			}
		}
	} else {
		netif_dormant_on(dev);
		while (pvc) {		 
			pvc_carrier(0, pvc);
			pvc->state.exist = pvc->state.active = 0;
			pvc->state.new = 0;
			if (!state(hdlc)->settings.dce)
				pvc->state.bandwidth = 0;
			pvc = pvc->next;
		}
	}
}