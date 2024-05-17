static int qib_tid_free(struct qib_ctxtdata *rcd, unsigned subctxt,
			const struct qib_tid_info *ti)
{
	int ret = 0;
	u32 tid, ctxttid, cnt, limit, tidcnt;
	struct qib_devdata *dd = rcd->dd;
	u64 __iomem *tidbase;
	unsigned long tidmap[8];

	if (!dd->pageshadow) {
		ret = -ENOMEM;
		goto done;
	}

	if (copy_from_user(tidmap, (void __user *)(unsigned long)ti->tidmap,
			   sizeof(tidmap))) {
		ret = -EFAULT;
		goto done;
	}

	ctxttid = rcd->ctxt * dd->rcvtidcnt;
	if (!rcd->subctxt_cnt)
		tidcnt = dd->rcvtidcnt;
	else if (!subctxt) {
		tidcnt = (dd->rcvtidcnt / rcd->subctxt_cnt) +
			 (dd->rcvtidcnt % rcd->subctxt_cnt);
		ctxttid += dd->rcvtidcnt - tidcnt;
	} else {
		tidcnt = dd->rcvtidcnt / rcd->subctxt_cnt;
		ctxttid += tidcnt * (subctxt - 1);
	}
	tidbase = (u64 __iomem *) ((char __iomem *)(dd->kregbase) +
				   dd->rcvtidbase +
				   ctxttid * sizeof(*tidbase));

	limit = sizeof(tidmap) * BITS_PER_BYTE;
	if (limit > tidcnt)
		 
		limit = tidcnt;
	tid = find_first_bit(tidmap, limit);
	for (cnt = 0; tid < limit; tid++) {
		 
		if (!test_bit(tid, tidmap))
			continue;
		cnt++;
		if (dd->pageshadow[ctxttid + tid]) {
			struct page *p;
			dma_addr_t phys;

			p = dd->pageshadow[ctxttid + tid];
			dd->pageshadow[ctxttid + tid] = NULL;
			phys = dd->physshadow[ctxttid + tid];
			dd->physshadow[ctxttid + tid] = dd->tidinvalid;
			 
			dd->f_put_tid(dd, &tidbase[tid],
				      RCVHQ_RCV_TYPE_EXPECTED, dd->tidinvalid);
			pci_unmap_page(dd->pcidev, phys, PAGE_SIZE,
				       PCI_DMA_FROMDEVICE);
			qib_release_user_pages(&p, 1);
		}
	}
done:
	return ret;
}