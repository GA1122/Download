static int get_base_info(struct file *fp, void __user *ubase, __u32 len)
{
	struct hfi1_base_info binfo;
	struct hfi1_filedata *fd = fp->private_data;
	struct hfi1_ctxtdata *uctxt = fd->uctxt;
	struct hfi1_devdata *dd = uctxt->dd;
	ssize_t sz;
	unsigned offset;
	int ret = 0;

	trace_hfi1_uctxtdata(uctxt->dd, uctxt);

	memset(&binfo, 0, sizeof(binfo));
	binfo.hw_version = dd->revision;
	binfo.sw_version = HFI1_KERN_SWVERSION;
	binfo.bthqp = kdeth_qp;
	binfo.jkey = uctxt->jkey;
	 
	offset = ((u64)uctxt->sc->hw_free -
		  (u64)dd->cr_base[uctxt->numa_id].va) % PAGE_SIZE;
	binfo.sc_credits_addr = HFI1_MMAP_TOKEN(PIO_CRED, uctxt->ctxt,
						fd->subctxt, offset);
	binfo.pio_bufbase = HFI1_MMAP_TOKEN(PIO_BUFS, uctxt->ctxt,
					    fd->subctxt,
					    uctxt->sc->base_addr);
	binfo.pio_bufbase_sop = HFI1_MMAP_TOKEN(PIO_BUFS_SOP,
						uctxt->ctxt,
						fd->subctxt,
						uctxt->sc->base_addr);
	binfo.rcvhdr_bufbase = HFI1_MMAP_TOKEN(RCV_HDRQ, uctxt->ctxt,
					       fd->subctxt,
					       uctxt->rcvhdrq);
	binfo.rcvegr_bufbase = HFI1_MMAP_TOKEN(RCV_EGRBUF, uctxt->ctxt,
					       fd->subctxt,
					       uctxt->egrbufs.rcvtids[0].phys);
	binfo.sdma_comp_bufbase = HFI1_MMAP_TOKEN(SDMA_COMP, uctxt->ctxt,
						 fd->subctxt, 0);
	 
	binfo.user_regbase = HFI1_MMAP_TOKEN(UREGS, uctxt->ctxt,
					    fd->subctxt, 0);
	offset = offset_in_page((((uctxt->ctxt - dd->first_user_ctxt) *
		    HFI1_MAX_SHARED_CTXTS) + fd->subctxt) *
		  sizeof(*dd->events));
	binfo.events_bufbase = HFI1_MMAP_TOKEN(EVENTS, uctxt->ctxt,
					      fd->subctxt,
					      offset);
	binfo.status_bufbase = HFI1_MMAP_TOKEN(STATUS, uctxt->ctxt,
					      fd->subctxt,
					      dd->status);
	if (HFI1_CAP_IS_USET(DMA_RTAIL))
		binfo.rcvhdrtail_base = HFI1_MMAP_TOKEN(RTAIL, uctxt->ctxt,
						       fd->subctxt, 0);
	if (uctxt->subctxt_cnt) {
		binfo.subctxt_uregbase = HFI1_MMAP_TOKEN(SUBCTXT_UREGS,
							uctxt->ctxt,
							fd->subctxt, 0);
		binfo.subctxt_rcvhdrbuf = HFI1_MMAP_TOKEN(SUBCTXT_RCV_HDRQ,
							 uctxt->ctxt,
							 fd->subctxt, 0);
		binfo.subctxt_rcvegrbuf = HFI1_MMAP_TOKEN(SUBCTXT_EGRBUF,
							 uctxt->ctxt,
							 fd->subctxt, 0);
	}
	sz = (len < sizeof(binfo)) ? len : sizeof(binfo);
	if (copy_to_user(ubase, &binfo, sz))
		ret = -EFAULT;
	return ret;
}