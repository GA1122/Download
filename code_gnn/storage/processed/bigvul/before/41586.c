static int irda_getsockopt(struct socket *sock, int level, int optname,
			   char __user *optval, int __user *optlen)
{
	struct sock *sk = sock->sk;
	struct irda_sock *self = irda_sk(sk);
	struct irda_device_list list;
	struct irda_device_info *discoveries;
	struct irda_ias_set *	ias_opt;	 
	struct ias_object *	ias_obj;	 
	struct ias_attrib *	ias_attr;	 
	int daddr = DEV_ADDR_ANY;	 
	int val = 0;
	int len = 0;
	int err = 0;
	int offset, total;

	pr_debug("%s(%p)\n", __func__, self);

	if (level != SOL_IRLMP)
		return -ENOPROTOOPT;

	if (get_user(len, optlen))
		return -EFAULT;

	if(len < 0)
		return -EINVAL;

	lock_sock(sk);

	switch (optname) {
	case IRLMP_ENUMDEVICES:

		 
		offset = sizeof(struct irda_device_list) -
			sizeof(struct irda_device_info);

		if (len < offset) {
			err = -EINVAL;
			goto out;
		}

		 
		discoveries = irlmp_get_discoveries(&list.len, self->mask.word,
						    self->nslots);
		 
		if (discoveries == NULL) {
			err = -EAGAIN;
			goto out;		 
		}

		 
		if (copy_to_user(optval, &list, offset))
			err = -EFAULT;

		 
		if (list.len > 2048) {
			err = -EINVAL;
			goto bed;
		}
		total = offset + (list.len * sizeof(struct irda_device_info));
		if (total > len)
			total = len;
		if (copy_to_user(optval+offset, discoveries, total - offset))
			err = -EFAULT;

		 
		if (put_user(total, optlen))
			err = -EFAULT;
bed:
		 
		kfree(discoveries);
		break;
	case IRLMP_MAX_SDU_SIZE:
		val = self->max_data_size;
		len = sizeof(int);
		if (put_user(len, optlen)) {
			err = -EFAULT;
			goto out;
		}

		if (copy_to_user(optval, &val, len)) {
			err = -EFAULT;
			goto out;
		}

		break;
	case IRLMP_IAS_GET:
		 

		 
		if (len != sizeof(struct irda_ias_set)) {
			err = -EINVAL;
			goto out;
		}

		ias_opt = kmalloc(sizeof(struct irda_ias_set), GFP_ATOMIC);
		if (ias_opt == NULL) {
			err = -ENOMEM;
			goto out;
		}

		 
		if (copy_from_user(ias_opt, optval, len)) {
			kfree(ias_opt);
			err = -EFAULT;
			goto out;
		}

		 
		if(ias_opt->irda_class_name[0] == '\0')
			ias_obj = self->ias_obj;
		else
			ias_obj = irias_find_object(ias_opt->irda_class_name);
		if(ias_obj == (struct ias_object *) NULL) {
			kfree(ias_opt);
			err = -EINVAL;
			goto out;
		}

		 
		ias_attr = irias_find_attrib(ias_obj,
					     ias_opt->irda_attrib_name);
		if(ias_attr == (struct ias_attrib *) NULL) {
			kfree(ias_opt);
			err = -EINVAL;
			goto out;
		}

		 
		err = irda_extract_ias_value(ias_opt, ias_attr->value);
		if(err) {
			kfree(ias_opt);
			goto out;
		}

		 
		if (copy_to_user(optval, ias_opt,
				 sizeof(struct irda_ias_set))) {
			kfree(ias_opt);
			err = -EFAULT;
			goto out;
		}
		 
		kfree(ias_opt);
		break;
	case IRLMP_IAS_QUERY:
		 

		 
		if (len != sizeof(struct irda_ias_set)) {
			err = -EINVAL;
			goto out;
		}

		ias_opt = kmalloc(sizeof(struct irda_ias_set), GFP_ATOMIC);
		if (ias_opt == NULL) {
			err = -ENOMEM;
			goto out;
		}

		 
		if (copy_from_user(ias_opt, optval, len)) {
			kfree(ias_opt);
			err = -EFAULT;
			goto out;
		}

		 
		if(self->daddr != DEV_ADDR_ANY) {
			 
			daddr = self->daddr;
		} else {
			 
			daddr = ias_opt->daddr;
			if((!daddr) || (daddr == DEV_ADDR_ANY)) {
				kfree(ias_opt);
				err = -EINVAL;
				goto out;
			}
		}

		 
		if (self->iriap) {
			net_warn_ratelimited("%s: busy with a previous query\n",
					     __func__);
			kfree(ias_opt);
			err = -EBUSY;
			goto out;
		}

		self->iriap = iriap_open(LSAP_ANY, IAS_CLIENT, self,
					 irda_getvalue_confirm);

		if (self->iriap == NULL) {
			kfree(ias_opt);
			err = -ENOMEM;
			goto out;
		}

		 
		self->errno = -EHOSTUNREACH;

		 
		iriap_getvaluebyclass_request(self->iriap,
					      self->saddr, daddr,
					      ias_opt->irda_class_name,
					      ias_opt->irda_attrib_name);

		 
		if (wait_event_interruptible(self->query_wait,
					     (self->iriap == NULL))) {
			 
			kfree(ias_opt);
			 
			err = -EHOSTUNREACH;
			goto out;
		}

		 
		if (self->errno)
		{
			kfree(ias_opt);
			 
			if((self->errno == IAS_CLASS_UNKNOWN) ||
			   (self->errno == IAS_ATTRIB_UNKNOWN))
				err = -EADDRNOTAVAIL;
			else
				err = -EHOSTUNREACH;

			goto out;
		}

		 
		err = irda_extract_ias_value(ias_opt, self->ias_result);
		if (self->ias_result)
			irias_delete_value(self->ias_result);
		if (err) {
			kfree(ias_opt);
			goto out;
		}

		 
		if (copy_to_user(optval, ias_opt,
				 sizeof(struct irda_ias_set))) {
			kfree(ias_opt);
			err = -EFAULT;
			goto out;
		}
		 
		kfree(ias_opt);
		break;
	case IRLMP_WAITDEVICE:
		 

		 
		if (len != sizeof(int)) {
			err = -EINVAL;
			goto out;
		}
		 
		if (get_user(val, (int __user *)optval)) {
			err = -EFAULT;
			goto out;
		}

		 
		irlmp_update_client(self->ckey, self->mask.word,
				    irda_selective_discovery_indication,
				    NULL, (void *) self);

		 
		irlmp_discovery_request(self->nslots);

		 
		if (!self->cachedaddr) {
			pr_debug("%s(), nothing discovered yet, going to sleep...\n",
				 __func__);

			 
			self->errno = 0;
			setup_timer(&self->watchdog, irda_discovery_timeout,
					(unsigned long)self);
			mod_timer(&self->watchdog,
				  jiffies + msecs_to_jiffies(val));

			 
			err = __wait_event_interruptible(self->query_wait,
			      (self->cachedaddr != 0 || self->errno == -ETIME));

			 
			del_timer(&(self->watchdog));

			pr_debug("%s(), ...waking up !\n", __func__);

			if (err != 0)
				goto out;
		}
		else
			pr_debug("%s(), found immediately !\n",
				 __func__);

		 
		irlmp_update_client(self->ckey, self->mask.word,
				    NULL, NULL, NULL);

		 
		if (!self->cachedaddr) {
			err = -EAGAIN;		 
			goto out;
		}
		daddr = self->cachedaddr;
		 
		self->cachedaddr = 0;

		 
		if (put_user(daddr, (int __user *)optval)) {
			err = -EFAULT;
			goto out;
		}

		break;
	default:
		err = -ENOPROTOOPT;
	}

out:

	release_sock(sk);

	return err;
}
