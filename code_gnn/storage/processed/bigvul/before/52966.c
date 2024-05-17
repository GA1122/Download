static int hfi1_file_close(struct inode *inode, struct file *fp)
{
	struct hfi1_filedata *fdata = fp->private_data;
	struct hfi1_ctxtdata *uctxt = fdata->uctxt;
	struct hfi1_devdata *dd;
	unsigned long flags, *ev;

	fp->private_data = NULL;

	if (!uctxt)
		goto done;

	hfi1_cdbg(PROC, "freeing ctxt %u:%u", uctxt->ctxt, fdata->subctxt);
	dd = uctxt->dd;
	mutex_lock(&hfi1_mutex);

	flush_wc();
	 
	hfi1_user_sdma_free_queues(fdata);

	 
	hfi1_put_proc_affinity(dd, fdata->rec_cpu_num);

	 
	ev = dd->events + ((uctxt->ctxt - dd->first_user_ctxt) *
			   HFI1_MAX_SHARED_CTXTS) + fdata->subctxt;
	*ev = 0;

	if (--uctxt->cnt) {
		uctxt->active_slaves &= ~(1 << fdata->subctxt);
		uctxt->subpid[fdata->subctxt] = 0;
		mutex_unlock(&hfi1_mutex);
		goto done;
	}

	spin_lock_irqsave(&dd->uctxt_lock, flags);
	 
	hfi1_rcvctrl(dd, HFI1_RCVCTRL_CTXT_DIS |
		     HFI1_RCVCTRL_TIDFLOW_DIS |
		     HFI1_RCVCTRL_INTRAVAIL_DIS |
		     HFI1_RCVCTRL_TAILUPD_DIS |
		     HFI1_RCVCTRL_ONE_PKT_EGR_DIS |
		     HFI1_RCVCTRL_NO_RHQ_DROP_DIS |
		     HFI1_RCVCTRL_NO_EGR_DROP_DIS, uctxt->ctxt);
	 
	hfi1_clear_ctxt_jkey(dd, uctxt->ctxt);
	 
	write_kctxt_csr(dd, uctxt->sc->hw_context, SEND_CTXT_CHECK_ENABLE,
			hfi1_pkt_default_send_ctxt_mask(dd, uctxt->sc->type));
	sc_disable(uctxt->sc);
	uctxt->pid = 0;
	spin_unlock_irqrestore(&dd->uctxt_lock, flags);

	dd->rcd[uctxt->ctxt] = NULL;

	hfi1_user_exp_rcv_free(fdata);
	hfi1_clear_ctxt_pkey(dd, uctxt->ctxt);

	uctxt->rcvwait_to = 0;
	uctxt->piowait_to = 0;
	uctxt->rcvnowait = 0;
	uctxt->pionowait = 0;
	uctxt->event_flags = 0;

	hfi1_stats.sps_ctxts--;
	if (++dd->freectxts == dd->num_user_contexts)
		aspm_enable_all(dd);
	mutex_unlock(&hfi1_mutex);
	hfi1_free_ctxtdata(dd, uctxt);
done:
	kfree(fdata);
	return 0;
}
