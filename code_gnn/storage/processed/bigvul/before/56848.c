static int abort_rpl(struct t3cdev *tdev, struct sk_buff *skb, void *ctx)
{
	struct iwch_ep *ep = ctx;
	unsigned long flags;
	int release = 0;

	PDBG("%s ep %p\n", __func__, ep);
	BUG_ON(!ep);

	 
	if (!test_and_set_bit(ABORT_REQ_IN_PROGRESS, &ep->com.flags)) {
		return CPL_RET_BUF_DONE;
	}

	spin_lock_irqsave(&ep->com.lock, flags);
	switch (ep->com.state) {
	case ABORTING:
		close_complete_upcall(ep);
		__state_set(&ep->com, DEAD);
		release = 1;
		break;
	default:
		printk(KERN_ERR "%s ep %p state %d\n",
		     __func__, ep, ep->com.state);
		break;
	}
	spin_unlock_irqrestore(&ep->com.lock, flags);

	if (release)
		release_ep_resources(ep);
	return CPL_RET_BUF_DONE;
}