static void process_work(struct work_struct *work)
{
	struct sk_buff *skb = NULL;
	void *ep;
	struct t3cdev *tdev;
	int ret;

	while ((skb = skb_dequeue(&rxq))) {
		ep = *((void **) (skb->cb));
		tdev = *((struct t3cdev **) (skb->cb + sizeof(void *)));
		ret = work_handlers[G_OPCODE(ntohl((__force __be32)skb->csum))](tdev, skb, ep);
		if (ret & CPL_RET_BUF_DONE)
			kfree_skb(skb);

		 
		put_ep((struct iwch_ep_common *)ep);
	}
}