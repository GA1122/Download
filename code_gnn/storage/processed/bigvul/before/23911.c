static int fr_ioctl(struct net_device *dev, struct ifreq *ifr)
{
	fr_proto __user *fr_s = ifr->ifr_settings.ifs_ifsu.fr;
	const size_t size = sizeof(fr_proto);
	fr_proto new_settings;
	hdlc_device *hdlc = dev_to_hdlc(dev);
	fr_proto_pvc pvc;
	int result;

	switch (ifr->ifr_settings.type) {
	case IF_GET_PROTO:
		if (dev_to_hdlc(dev)->proto != &proto)  
			return -EINVAL;
		ifr->ifr_settings.type = IF_PROTO_FR;
		if (ifr->ifr_settings.size < size) {
			ifr->ifr_settings.size = size;  
			return -ENOBUFS;
		}
		if (copy_to_user(fr_s, &state(hdlc)->settings, size))
			return -EFAULT;
		return 0;

	case IF_PROTO_FR:
		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (dev->flags & IFF_UP)
			return -EBUSY;

		if (copy_from_user(&new_settings, fr_s, size))
			return -EFAULT;

		if (new_settings.lmi == LMI_DEFAULT)
			new_settings.lmi = LMI_ANSI;

		if ((new_settings.lmi != LMI_NONE &&
		     new_settings.lmi != LMI_ANSI &&
		     new_settings.lmi != LMI_CCITT &&
		     new_settings.lmi != LMI_CISCO) ||
		    new_settings.t391 < 1 ||
		    new_settings.t392 < 2 ||
		    new_settings.n391 < 1 ||
		    new_settings.n392 < 1 ||
		    new_settings.n393 < new_settings.n392 ||
		    new_settings.n393 > 32 ||
		    (new_settings.dce != 0 &&
		     new_settings.dce != 1))
			return -EINVAL;

		result=hdlc->attach(dev, ENCODING_NRZ,PARITY_CRC16_PR1_CCITT);
		if (result)
			return result;

		if (dev_to_hdlc(dev)->proto != &proto) {  
			result = attach_hdlc_protocol(dev, &proto,
						      sizeof(struct frad_state));
			if (result)
				return result;
			state(hdlc)->first_pvc = NULL;
			state(hdlc)->dce_pvc_count = 0;
		}
		memcpy(&state(hdlc)->settings, &new_settings, size);
		dev->type = ARPHRD_FRAD;
		return 0;

	case IF_PROTO_FR_ADD_PVC:
	case IF_PROTO_FR_DEL_PVC:
	case IF_PROTO_FR_ADD_ETH_PVC:
	case IF_PROTO_FR_DEL_ETH_PVC:
		if (dev_to_hdlc(dev)->proto != &proto)  
			return -EINVAL;

		if (!capable(CAP_NET_ADMIN))
			return -EPERM;

		if (copy_from_user(&pvc, ifr->ifr_settings.ifs_ifsu.fr_pvc,
				   sizeof(fr_proto_pvc)))
			return -EFAULT;

		if (pvc.dlci <= 0 || pvc.dlci >= 1024)
			return -EINVAL;	 

		if (ifr->ifr_settings.type == IF_PROTO_FR_ADD_ETH_PVC ||
		    ifr->ifr_settings.type == IF_PROTO_FR_DEL_ETH_PVC)
			result = ARPHRD_ETHER;  
		else
			result = ARPHRD_DLCI;

		if (ifr->ifr_settings.type == IF_PROTO_FR_ADD_PVC ||
		    ifr->ifr_settings.type == IF_PROTO_FR_ADD_ETH_PVC)
			return fr_add_pvc(dev, pvc.dlci, result);
		else
			return fr_del_pvc(hdlc, pvc.dlci, result);
	}

	return -EINVAL;
}