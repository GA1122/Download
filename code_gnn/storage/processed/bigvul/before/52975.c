static int manage_rcvq(struct hfi1_ctxtdata *uctxt, unsigned subctxt,
		       int start_stop)
{
	struct hfi1_devdata *dd = uctxt->dd;
	unsigned int rcvctrl_op;

	if (subctxt)
		goto bail;
	 
	if (start_stop) {
		 
		if (uctxt->rcvhdrtail_kvaddr)
			clear_rcvhdrtail(uctxt);
		rcvctrl_op = HFI1_RCVCTRL_CTXT_ENB;
	} else {
		rcvctrl_op = HFI1_RCVCTRL_CTXT_DIS;
	}
	hfi1_rcvctrl(dd, rcvctrl_op, uctxt->ctxt);
	 
bail:
	return 0;
}
