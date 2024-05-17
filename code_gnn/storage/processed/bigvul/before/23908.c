static int fr_del_pvc(hdlc_device *hdlc, unsigned int dlci, int type)
{
	pvc_device *pvc;
	struct net_device *dev;

	if ((pvc = find_pvc(hdlc, dlci)) == NULL)
		return -ENOENT;

	if ((dev = *get_dev_p(pvc, type)) == NULL)
		return -ENOENT;

	if (dev->flags & IFF_UP)
		return -EBUSY;		 

	unregister_netdevice(dev);  
	*get_dev_p(pvc, type) = NULL;

	if (!pvc_is_used(pvc)) {
		state(hdlc)->dce_pvc_count--;
		state(hdlc)->dce_changed = 1;
	}
	delete_unused_pvcs(hdlc);
	return 0;
}