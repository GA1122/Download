static int airo_get_scan(struct net_device *dev,
			 struct iw_request_info *info,
			 struct iw_point *dwrq,
			 char *extra)
{
	struct airo_info *ai = dev->ml_priv;
	BSSListElement *net;
	int err = 0;
	char *current_ev = extra;

	 
	if (ai->scan_timeout > 0)
		return -EAGAIN;

	if (down_interruptible(&ai->sem))
		return -EAGAIN;

	list_for_each_entry (net, &ai->network_list, list) {
		 
		current_ev = airo_translate_scan(dev, info, current_ev,
						 extra + dwrq->length,
						 &net->bss);

		 
		if((extra + dwrq->length - current_ev) <= IW_EV_ADDR_LEN) {
			 
			err = -E2BIG;
			goto out;
		}
	}

	 
	dwrq->length = (current_ev - extra);
	dwrq->flags = 0;	 

out:
	up(&ai->sem);
	return err;
}
