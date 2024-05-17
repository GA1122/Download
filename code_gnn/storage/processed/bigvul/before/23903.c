static pvc_device* add_pvc(struct net_device *dev, u16 dlci)
{
	hdlc_device *hdlc = dev_to_hdlc(dev);
	pvc_device *pvc, **pvc_p = &state(hdlc)->first_pvc;

	while (*pvc_p) {
		if ((*pvc_p)->dlci == dlci)
			return *pvc_p;
		if ((*pvc_p)->dlci > dlci)
			break;	 
		pvc_p = &(*pvc_p)->next;
	}

	pvc = kzalloc(sizeof(pvc_device), GFP_ATOMIC);
#ifdef DEBUG_PVC
	printk(KERN_DEBUG "add_pvc: allocated pvc %p, frad %p\n", pvc, dev);
#endif
	if (!pvc)
		return NULL;

	pvc->dlci = dlci;
	pvc->frad = dev;
	pvc->next = *pvc_p;	 
	*pvc_p = pvc;
	return pvc;
}