static int orinoco_ioctl_setessid(struct net_device *dev,
				  struct iw_request_info *info,
				  struct iw_point *erq,
				  char *essidbuf)
{
	struct orinoco_private *priv = ndev_priv(dev);
	unsigned long flags;

	 

	 
	if (erq->length > IW_ESSID_MAX_SIZE)
		return -E2BIG;

	if (orinoco_lock(priv, &flags) != 0)
		return -EBUSY;

	 
	memset(priv->desired_essid, 0, sizeof(priv->desired_essid));

	 
	if (erq->flags)
		memcpy(priv->desired_essid, essidbuf, erq->length);

	orinoco_unlock(priv, &flags);

	return -EINPROGRESS;		 
}
