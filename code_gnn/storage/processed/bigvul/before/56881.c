static int pass_accept_req(struct t3cdev *tdev, struct sk_buff *skb, void *ctx)
{
	struct iwch_ep *child_ep, *parent_ep = ctx;
	struct cpl_pass_accept_req *req = cplhdr(skb);
	unsigned int hwtid = GET_TID(req);
	struct dst_entry *dst;
	struct l2t_entry *l2t;
	struct rtable *rt;
	struct iff_mac tim;

	PDBG("%s parent ep %p tid %u\n", __func__, parent_ep, hwtid);

	if (state_read(&parent_ep->com) != LISTEN) {
		printk(KERN_ERR "%s - listening ep not in LISTEN\n",
		       __func__);
		goto reject;
	}

	 
	tim.mac_addr = req->dst_mac;
	tim.vlan_tag = ntohs(req->vlan_tag);
	if (tdev->ctl(tdev, GET_IFF_FROM_MAC, &tim) < 0 || !tim.dev) {
		printk(KERN_ERR "%s bad dst mac %pM\n",
			__func__, req->dst_mac);
		goto reject;
	}

	 
	rt = find_route(tdev,
			req->local_ip,
			req->peer_ip,
			req->local_port,
			req->peer_port, G_PASS_OPEN_TOS(ntohl(req->tos_tid)));
	if (!rt) {
		printk(KERN_ERR MOD "%s - failed to find dst entry!\n",
		       __func__);
		goto reject;
	}
	dst = &rt->dst;
	l2t = t3_l2t_get(tdev, dst, NULL, &req->peer_ip);
	if (!l2t) {
		printk(KERN_ERR MOD "%s - failed to allocate l2t entry!\n",
		       __func__);
		dst_release(dst);
		goto reject;
	}
	child_ep = alloc_ep(sizeof(*child_ep), GFP_KERNEL);
	if (!child_ep) {
		printk(KERN_ERR MOD "%s - failed to allocate ep entry!\n",
		       __func__);
		l2t_release(tdev, l2t);
		dst_release(dst);
		goto reject;
	}
	state_set(&child_ep->com, CONNECTING);
	child_ep->com.tdev = tdev;
	child_ep->com.cm_id = NULL;
	child_ep->com.local_addr.sin_family = PF_INET;
	child_ep->com.local_addr.sin_port = req->local_port;
	child_ep->com.local_addr.sin_addr.s_addr = req->local_ip;
	child_ep->com.remote_addr.sin_family = PF_INET;
	child_ep->com.remote_addr.sin_port = req->peer_port;
	child_ep->com.remote_addr.sin_addr.s_addr = req->peer_ip;
	get_ep(&parent_ep->com);
	child_ep->parent_ep = parent_ep;
	child_ep->tos = G_PASS_OPEN_TOS(ntohl(req->tos_tid));
	child_ep->l2t = l2t;
	child_ep->dst = dst;
	child_ep->hwtid = hwtid;
	init_timer(&child_ep->timer);
	cxgb3_insert_tid(tdev, &t3c_client, child_ep, hwtid);
	accept_cr(child_ep, req->peer_ip, skb);
	goto out;
reject:
	reject_cr(tdev, hwtid, req->peer_ip, skb);
out:
	return CPL_RET_BUF_DONE;
}