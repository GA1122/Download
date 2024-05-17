static int allocate_ctxt(struct file *fp, struct hfi1_devdata *dd,
			 struct hfi1_user_info *uinfo)
{
	struct hfi1_filedata *fd = fp->private_data;
	struct hfi1_ctxtdata *uctxt;
	unsigned ctxt;
	int ret, numa;

	if (dd->flags & HFI1_FROZEN) {
		 
		return -EIO;
	}

	for (ctxt = dd->first_user_ctxt; ctxt < dd->num_rcv_contexts; ctxt++)
		if (!dd->rcd[ctxt])
			break;

	if (ctxt == dd->num_rcv_contexts)
		return -EBUSY;

	fd->rec_cpu_num = hfi1_get_proc_affinity(dd, -1);
	if (fd->rec_cpu_num != -1)
		numa = cpu_to_node(fd->rec_cpu_num);
	else
		numa = numa_node_id();
	uctxt = hfi1_create_ctxtdata(dd->pport, ctxt, numa);
	if (!uctxt) {
		dd_dev_err(dd,
			   "Unable to allocate ctxtdata memory, failing open\n");
		return -ENOMEM;
	}
	hfi1_cdbg(PROC, "[%u:%u] pid %u assigned to CPU %d (NUMA %u)",
		  uctxt->ctxt, fd->subctxt, current->pid, fd->rec_cpu_num,
		  uctxt->numa_id);

	 
	uctxt->sc = sc_alloc(dd, SC_USER, uctxt->rcvhdrqentsize,
			     uctxt->dd->node);
	if (!uctxt->sc)
		return -ENOMEM;

	hfi1_cdbg(PROC, "allocated send context %u(%u)\n", uctxt->sc->sw_index,
		  uctxt->sc->hw_context);
	ret = sc_enable(uctxt->sc);
	if (ret)
		return ret;
	 
	if (uinfo->subctxt_cnt && !fd->subctxt) {
		ret = init_subctxts(uctxt, uinfo);
		 
		if (ret)
			return ret;
	}
	uctxt->userversion = uinfo->userversion;
	uctxt->pid = current->pid;
	uctxt->flags = HFI1_CAP_UGET(MASK);
	init_waitqueue_head(&uctxt->wait);
	strlcpy(uctxt->comm, current->comm, sizeof(uctxt->comm));
	memcpy(uctxt->uuid, uinfo->uuid, sizeof(uctxt->uuid));
	uctxt->jkey = generate_jkey(current_uid());
	INIT_LIST_HEAD(&uctxt->sdma_queues);
	spin_lock_init(&uctxt->sdma_qlock);
	hfi1_stats.sps_ctxts++;
	 
	if (dd->freectxts-- == dd->num_user_contexts)
		aspm_disable_all(dd);
	fd->uctxt = uctxt;

	return 0;
}
