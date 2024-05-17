int iwch_connect(struct iw_cm_id *cm_id, struct iw_cm_conn_param *conn_param)
{
	struct iwch_dev *h = to_iwch_dev(cm_id->device);
	struct iwch_ep *ep;
	struct rtable *rt;
	int err = 0;
	struct sockaddr_in *laddr = (struct sockaddr_in *)&cm_id->local_addr;
	struct sockaddr_in *raddr = (struct sockaddr_in *)&cm_id->remote_addr;

	if (cm_id->remote_addr.ss_family != PF_INET) {
		err = -ENOSYS;
		goto out;
	}

	if (is_loopback_dst(cm_id)) {
		err = -ENOSYS;
		goto out;
	}

	ep = alloc_ep(sizeof(*ep), GFP_KERNEL);
	if (!ep) {
		printk(KERN_ERR MOD "%s - cannot alloc ep.\n", __func__);
		err = -ENOMEM;
		goto out;
	}
	init_timer(&ep->timer);
	ep->plen = conn_param->private_data_len;
	if (ep->plen)
		memcpy(ep->mpa_pkt + sizeof(struct mpa_message),
		       conn_param->private_data, ep->plen);
	ep->ird = conn_param->ird;
	ep->ord = conn_param->ord;

	if (peer2peer && ep->ord == 0)
		ep->ord = 1;

	ep->com.tdev = h->rdev.t3cdev_p;

	cm_id->add_ref(cm_id);
	ep->com.cm_id = cm_id;
	ep->com.qp = get_qhp(h, conn_param->qpn);
	BUG_ON(!ep->com.qp);
	PDBG("%s qpn 0x%x qp %p cm_id %p\n", __func__, conn_param->qpn,
	     ep->com.qp, cm_id);

	 
	ep->atid = cxgb3_alloc_atid(h->rdev.t3cdev_p, &t3c_client, ep);
	if (ep->atid == -1) {
		printk(KERN_ERR MOD "%s - cannot alloc atid.\n", __func__);
		err = -ENOMEM;
		goto fail2;
	}

	 
	rt = find_route(h->rdev.t3cdev_p, laddr->sin_addr.s_addr,
			raddr->sin_addr.s_addr, laddr->sin_port,
			raddr->sin_port, IPTOS_LOWDELAY);
	if (!rt) {
		printk(KERN_ERR MOD "%s - cannot find route.\n", __func__);
		err = -EHOSTUNREACH;
		goto fail3;
	}
	ep->dst = &rt->dst;
	ep->l2t = t3_l2t_get(ep->com.tdev, ep->dst, NULL,
			     &raddr->sin_addr.s_addr);
	if (!ep->l2t) {
		printk(KERN_ERR MOD "%s - cannot alloc l2e.\n", __func__);
		err = -ENOMEM;
		goto fail4;
	}

	state_set(&ep->com, CONNECTING);
	ep->tos = IPTOS_LOWDELAY;
	memcpy(&ep->com.local_addr, &cm_id->local_addr,
	       sizeof(ep->com.local_addr));
	memcpy(&ep->com.remote_addr, &cm_id->remote_addr,
	       sizeof(ep->com.remote_addr));

	 
	err = send_connect(ep);
	if (!err)
		goto out;

	l2t_release(h->rdev.t3cdev_p, ep->l2t);
fail4:
	dst_release(ep->dst);
fail3:
	cxgb3_free_atid(ep->com.tdev, ep->atid);
fail2:
	cm_id->rem_ref(cm_id);
	put_ep(&ep->com);
out:
	return err;
}