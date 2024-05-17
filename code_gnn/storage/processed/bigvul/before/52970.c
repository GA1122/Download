int hfi1_set_uevent_bits(struct hfi1_pportdata *ppd, const int evtbit)
{
	struct hfi1_ctxtdata *uctxt;
	struct hfi1_devdata *dd = ppd->dd;
	unsigned ctxt;
	int ret = 0;
	unsigned long flags;

	if (!dd->events) {
		ret = -EINVAL;
		goto done;
	}

	spin_lock_irqsave(&dd->uctxt_lock, flags);
	for (ctxt = dd->first_user_ctxt; ctxt < dd->num_rcv_contexts;
	     ctxt++) {
		uctxt = dd->rcd[ctxt];
		if (uctxt) {
			unsigned long *evs = dd->events +
				(uctxt->ctxt - dd->first_user_ctxt) *
				HFI1_MAX_SHARED_CTXTS;
			int i;
			 
			set_bit(evtbit, evs);
			for (i = 1; i < uctxt->subctxt_cnt; i++)
				set_bit(evtbit, evs + i);
		}
	}
	spin_unlock_irqrestore(&dd->uctxt_lock, flags);
done:
	return ret;
}