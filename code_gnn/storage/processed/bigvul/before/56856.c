static int close_con_rpl(struct t3cdev *tdev, struct sk_buff *skb, void *ctx)
{
	struct iwch_ep *ep = ctx;
	struct iwch_qp_attributes attrs;
	unsigned long flags;
	int release = 0;

	PDBG("%s ep %p\n", __func__, ep);
	BUG_ON(!ep);

	 
	spin_lock_irqsave(&ep->com.lock, flags);
	switch (ep->com.state) {
	case CLOSING:
		__state_set(&ep->com, MORIBUND);
		break;
	case MORIBUND:
		stop_ep_timer(ep);
		if ((ep->com.cm_id) && (ep->com.qp)) {
			attrs.next_state = IWCH_QP_STATE_IDLE;
			iwch_modify_qp(ep->com.qp->rhp,
					     ep->com.qp,
					     IWCH_QP_ATTR_NEXT_STATE,
					     &attrs, 1);
		}
		close_complete_upcall(ep);
		__state_set(&ep->com, DEAD);
		release = 1;
		break;
	case ABORTING:
	case DEAD:
		break;
	default:
		BUG_ON(1);
		break;
	}
	spin_unlock_irqrestore(&ep->com.lock, flags);
	if (release)
		release_ep_resources(ep);
	return CPL_RET_BUF_DONE;
}