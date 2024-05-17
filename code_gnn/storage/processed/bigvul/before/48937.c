sch_handle_egress(struct sk_buff *skb, int *ret, struct net_device *dev)
{
	struct tcf_proto *cl = rcu_dereference_bh(dev->egress_cl_list);
	struct tcf_result cl_res;

	if (!cl)
		return skb;

	 
	qdisc_bstats_cpu_update(cl->q, skb);

	switch (tc_classify(skb, cl, &cl_res, false)) {
	case TC_ACT_OK:
	case TC_ACT_RECLASSIFY:
		skb->tc_index = TC_H_MIN(cl_res.classid);
		break;
	case TC_ACT_SHOT:
		qdisc_qstats_cpu_drop(cl->q);
		*ret = NET_XMIT_DROP;
		goto drop;
	case TC_ACT_STOLEN:
	case TC_ACT_QUEUED:
		*ret = NET_XMIT_SUCCESS;
drop:
		kfree_skb(skb);
		return NULL;
	case TC_ACT_REDIRECT:
		 
		skb_do_redirect(skb);
		*ret = NET_XMIT_SUCCESS;
		return NULL;
	default:
		break;
	}

	return skb;
}