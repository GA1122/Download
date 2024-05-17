static int irda_setsockopt(struct socket *sock, int level, int optname,
			   char __user *optval, unsigned int optlen)
{
	struct sock *sk = sock->sk;
	struct irda_sock *self = irda_sk(sk);
	struct irda_ias_set    *ias_opt;
	struct ias_object      *ias_obj;
	struct ias_attrib *	ias_attr;	 
	int opt, free_ias = 0, err = 0;

	pr_debug("%s(%p)\n", __func__, self);

	if (level != SOL_IRLMP)
		return -ENOPROTOOPT;

	lock_sock(sk);

	switch (optname) {
	case IRLMP_IAS_SET:
		 

		if (optlen != sizeof(struct irda_ias_set)) {
			err = -EINVAL;
			goto out;
		}

		ias_opt = kmalloc(sizeof(struct irda_ias_set), GFP_ATOMIC);
		if (ias_opt == NULL) {
			err = -ENOMEM;
			goto out;
		}

		 
		if (copy_from_user(ias_opt, optval, optlen)) {
			kfree(ias_opt);
			err = -EFAULT;
			goto out;
		}

		 
		if(ias_opt->irda_class_name[0] == '\0') {
			if(self->ias_obj == NULL) {
				kfree(ias_opt);
				err = -EINVAL;
				goto out;
			}
			ias_obj = self->ias_obj;
		} else
			ias_obj = irias_find_object(ias_opt->irda_class_name);

		 
		if((!capable(CAP_NET_ADMIN)) &&
		   ((ias_obj == NULL) || (ias_obj != self->ias_obj))) {
			kfree(ias_opt);
			err = -EPERM;
			goto out;
		}

		 
		if(ias_obj == (struct ias_object *) NULL) {
			 
			ias_obj = irias_new_object(ias_opt->irda_class_name,
						   jiffies);
			if (ias_obj == NULL) {
				kfree(ias_opt);
				err = -ENOMEM;
				goto out;
			}
			free_ias = 1;
		}

		 
		if(irias_find_attrib(ias_obj, ias_opt->irda_attrib_name)) {
			kfree(ias_opt);
			if (free_ias) {
				kfree(ias_obj->name);
				kfree(ias_obj);
			}
			err = -EINVAL;
			goto out;
		}

		 
		switch(ias_opt->irda_attrib_type) {
		case IAS_INTEGER:
			 
			irias_add_integer_attrib(
				ias_obj,
				ias_opt->irda_attrib_name,
				ias_opt->attribute.irda_attrib_int,
				IAS_USER_ATTR);
			break;
		case IAS_OCT_SEQ:
			 
			if(ias_opt->attribute.irda_attrib_octet_seq.len >
			   IAS_MAX_OCTET_STRING) {
				kfree(ias_opt);
				if (free_ias) {
					kfree(ias_obj->name);
					kfree(ias_obj);
				}

				err = -EINVAL;
				goto out;
			}
			 
			irias_add_octseq_attrib(
			      ias_obj,
			      ias_opt->irda_attrib_name,
			      ias_opt->attribute.irda_attrib_octet_seq.octet_seq,
			      ias_opt->attribute.irda_attrib_octet_seq.len,
			      IAS_USER_ATTR);
			break;
		case IAS_STRING:
			 
			 
			 
			 
			ias_opt->attribute.irda_attrib_string.string[ias_opt->attribute.irda_attrib_string.len] = '\0';
			 
			irias_add_string_attrib(
				ias_obj,
				ias_opt->irda_attrib_name,
				ias_opt->attribute.irda_attrib_string.string,
				IAS_USER_ATTR);
			break;
		default :
			kfree(ias_opt);
			if (free_ias) {
				kfree(ias_obj->name);
				kfree(ias_obj);
			}
			err = -EINVAL;
			goto out;
		}
		irias_insert_object(ias_obj);
		kfree(ias_opt);
		break;
	case IRLMP_IAS_DEL:
		 

		if (optlen != sizeof(struct irda_ias_set)) {
			err = -EINVAL;
			goto out;
		}

		ias_opt = kmalloc(sizeof(struct irda_ias_set), GFP_ATOMIC);
		if (ias_opt == NULL) {
			err = -ENOMEM;
			goto out;
		}

		 
		if (copy_from_user(ias_opt, optval, optlen)) {
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

		 
		if((!capable(CAP_NET_ADMIN)) &&
		   ((ias_obj == NULL) || (ias_obj != self->ias_obj))) {
			kfree(ias_opt);
			err = -EPERM;
			goto out;
		}

		 
		ias_attr = irias_find_attrib(ias_obj,
					     ias_opt->irda_attrib_name);
		if(ias_attr == (struct ias_attrib *) NULL) {
			kfree(ias_opt);
			err = -EINVAL;
			goto out;
		}

		 
		if(ias_attr->value->owner != IAS_USER_ATTR) {
			pr_debug("%s(), attempting to delete a kernel attribute\n",
				 __func__);
			kfree(ias_opt);
			err = -EPERM;
			goto out;
		}

		 
		irias_delete_attrib(ias_obj, ias_attr, 1);
		kfree(ias_opt);
		break;
	case IRLMP_MAX_SDU_SIZE:
		if (optlen < sizeof(int)) {
			err = -EINVAL;
			goto out;
		}

		if (get_user(opt, (int __user *)optval)) {
			err = -EFAULT;
			goto out;
		}

		 
		if (sk->sk_type != SOCK_SEQPACKET) {
			pr_debug("%s(), setting max_sdu_size = %d\n",
				 __func__, opt);
			self->max_sdu_size_rx = opt;
		} else {
			net_warn_ratelimited("%s: not allowed to set MAXSDUSIZE for this socket type!\n",
					     __func__);
			err = -ENOPROTOOPT;
			goto out;
		}
		break;
	case IRLMP_HINTS_SET:
		if (optlen < sizeof(int)) {
			err = -EINVAL;
			goto out;
		}

		 
		if (get_user(opt, (int __user *)optval)) {
			err = -EFAULT;
			goto out;
		}

		 
		irlmp_unregister_service(self->skey);

		self->skey = irlmp_register_service((__u16) opt);
		break;
	case IRLMP_HINT_MASK_SET:
		 
		if (optlen < sizeof(int)) {
			err = -EINVAL;
			goto out;
		}

		 
		if (get_user(opt, (int __user *)optval)) {
			err = -EFAULT;
			goto out;
		}

		 
		self->mask.word = (__u16) opt;
		 
		self->mask.word &= 0x7f7f;
		 
		if(!self->mask.word)
			self->mask.word = 0xFFFF;

		break;
	default:
		err = -ENOPROTOOPT;
		break;
	}

out:
	release_sock(sk);

	return err;
}
