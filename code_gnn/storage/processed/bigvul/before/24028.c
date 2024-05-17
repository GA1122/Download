static int flashcard(struct net_device *dev, aironet_ioctl *comp) {
	int z;

	 
	if (!capable(CAP_NET_ADMIN))
		return -EPERM;

	switch(comp->command)
	{
	case AIROFLSHRST:
		return cmdreset((struct airo_info *)dev->ml_priv);

	case AIROFLSHSTFL:
		if (!AIRO_FLASH(dev) &&
		    (AIRO_FLASH(dev) = kmalloc(FLASHSIZE, GFP_KERNEL)) == NULL)
			return -ENOMEM;
		return setflashmode((struct airo_info *)dev->ml_priv);

	case AIROFLSHGCHR:  
		if(comp->len != sizeof(int))
			return -EINVAL;
		if (copy_from_user(&z,comp->data,comp->len))
			return -EFAULT;
		return flashgchar((struct airo_info *)dev->ml_priv, z, 8000);

	case AIROFLSHPCHR:  
		if(comp->len != sizeof(int))
			return -EINVAL;
		if (copy_from_user(&z,comp->data,comp->len))
			return -EFAULT;
		return flashpchar((struct airo_info *)dev->ml_priv, z, 8000);

	case AIROFLPUTBUF:  
		if (!AIRO_FLASH(dev))
			return -ENOMEM;
		if(comp->len > FLASHSIZE)
			return -EINVAL;
		if (copy_from_user(AIRO_FLASH(dev), comp->data, comp->len))
			return -EFAULT;

		flashputbuf((struct airo_info *)dev->ml_priv);
		return 0;

	case AIRORESTART:
		if (flashrestart((struct airo_info *)dev->ml_priv, dev))
			return -EIO;
		return 0;
	}
	return -EINVAL;
}