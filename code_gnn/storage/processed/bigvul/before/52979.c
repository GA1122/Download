static int setup_ctxt(struct file *fp)
{
	struct hfi1_filedata *fd = fp->private_data;
	struct hfi1_ctxtdata *uctxt = fd->uctxt;
	struct hfi1_devdata *dd = uctxt->dd;
	int ret = 0;

	 
	if (!uctxt->subctxt_cnt || !fd->subctxt) {
		ret = hfi1_init_ctxt(uctxt->sc);
		if (ret)
			goto done;

		 
		ret = hfi1_create_rcvhdrq(dd, uctxt);
		if (ret)
			goto done;
		ret = hfi1_setup_eagerbufs(uctxt);
		if (ret)
			goto done;
		if (uctxt->subctxt_cnt && !fd->subctxt) {
			ret = setup_subctxt(uctxt);
			if (ret)
				goto done;
		}
	} else {
		ret = wait_event_interruptible(uctxt->wait, !test_bit(
					       HFI1_CTXT_MASTER_UNINIT,
					       &uctxt->event_flags));
		if (ret)
			goto done;
	}

	ret = hfi1_user_sdma_alloc_queues(uctxt, fp);
	if (ret)
		goto done;
	 
	ret = hfi1_user_exp_rcv_init(fp);
	if (ret)
		goto done;

	set_bit(HFI1_CTXT_SETUP_DONE, &uctxt->event_flags);
done:
	return ret;
}
