static int user_init(struct file *fp)
{
	unsigned int rcvctrl_ops = 0;
	struct hfi1_filedata *fd = fp->private_data;
	struct hfi1_ctxtdata *uctxt = fd->uctxt;

	 
	if (!test_bit(HFI1_CTXT_SETUP_DONE, &uctxt->event_flags))
		return -EFAULT;

	 
	uctxt->urgent = 0;
	uctxt->urgent_poll = 0;

	 
	if (uctxt->rcvhdrtail_kvaddr)
		clear_rcvhdrtail(uctxt);

	 
	hfi1_set_ctxt_jkey(uctxt->dd, uctxt->ctxt, uctxt->jkey);

	rcvctrl_ops = HFI1_RCVCTRL_CTXT_ENB;
	if (HFI1_CAP_KGET_MASK(uctxt->flags, HDRSUPP))
		rcvctrl_ops |= HFI1_RCVCTRL_TIDFLOW_ENB;
	 
	if (!HFI1_CAP_KGET_MASK(uctxt->flags, MULTI_PKT_EGR))
		rcvctrl_ops |= HFI1_RCVCTRL_ONE_PKT_EGR_ENB;
	if (HFI1_CAP_KGET_MASK(uctxt->flags, NODROP_EGR_FULL))
		rcvctrl_ops |= HFI1_RCVCTRL_NO_EGR_DROP_ENB;
	if (HFI1_CAP_KGET_MASK(uctxt->flags, NODROP_RHQ_FULL))
		rcvctrl_ops |= HFI1_RCVCTRL_NO_RHQ_DROP_ENB;
	 
	if (HFI1_CAP_KGET_MASK(uctxt->flags, DMA_RTAIL))
		rcvctrl_ops |= HFI1_RCVCTRL_TAILUPD_ENB;
	else
		rcvctrl_ops |= HFI1_RCVCTRL_TAILUPD_DIS;
	hfi1_rcvctrl(uctxt->dd, rcvctrl_ops, uctxt->ctxt);

	 
	if (uctxt->subctxt_cnt) {
		clear_bit(HFI1_CTXT_MASTER_UNINIT, &uctxt->event_flags);
		wake_up(&uctxt->wait);
	}

	return 0;
}
