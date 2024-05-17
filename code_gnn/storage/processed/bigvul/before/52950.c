static int qib_tid_update(struct qib_ctxtdata *rcd, struct file *fp,
			  const struct qib_tid_info *ti)
{
	int ret = 0, ntids;
	u32 tid, ctxttid, cnt, i, tidcnt, tidoff;
	u16 *tidlist;
	struct qib_devdata *dd = rcd->dd;
	u64 physaddr;
	unsigned long vaddr;
	u64 __iomem *tidbase;
	unsigned long tidmap[8];
	struct page **pagep = NULL;
	unsigned subctxt = subctxt_fp(fp);

	if (!dd->pageshadow) {
		ret = -ENOMEM;
		goto done;
	}

	cnt = ti->tidcnt;
	if (!cnt) {
		ret = -EFAULT;
		goto done;
	}
	ctxttid = rcd->ctxt * dd->rcvtidcnt;
	if (!rcd->subctxt_cnt) {
		tidcnt = dd->rcvtidcnt;
		tid = rcd->tidcursor;
		tidoff = 0;
	} else if (!subctxt) {
		tidcnt = (dd->rcvtidcnt / rcd->subctxt_cnt) +
			 (dd->rcvtidcnt % rcd->subctxt_cnt);
		tidoff = dd->rcvtidcnt - tidcnt;
		ctxttid += tidoff;
		tid = tidcursor_fp(fp);
	} else {
		tidcnt = dd->rcvtidcnt / rcd->subctxt_cnt;
		tidoff = tidcnt * (subctxt - 1);
		ctxttid += tidoff;
		tid = tidcursor_fp(fp);
	}
	if (cnt > tidcnt) {
		 
		qib_devinfo(dd->pcidev,
			"Process tried to allocate %u TIDs, only trying max (%u)\n",
			cnt, tidcnt);
		cnt = tidcnt;
	}
	pagep = (struct page **) rcd->tid_pg_list;
	tidlist = (u16 *) &pagep[dd->rcvtidcnt];
	pagep += tidoff;
	tidlist += tidoff;

	memset(tidmap, 0, sizeof(tidmap));
	 
	ntids = tidcnt;
	tidbase = (u64 __iomem *) (((char __iomem *) dd->kregbase) +
				   dd->rcvtidbase +
				   ctxttid * sizeof(*tidbase));

	 
	vaddr = ti->tidvaddr;
	if (!access_ok(VERIFY_WRITE, (void __user *) vaddr,
		       cnt * PAGE_SIZE)) {
		ret = -EFAULT;
		goto done;
	}
	ret = qib_get_user_pages(vaddr, cnt, pagep);
	if (ret) {
		 
		qib_devinfo(
			dd->pcidev,
			"Failed to lock addr %p, %u pages: errno %d\n",
			(void *) vaddr, cnt, -ret);
		goto done;
	}
	for (i = 0; i < cnt; i++, vaddr += PAGE_SIZE) {
		for (; ntids--; tid++) {
			if (tid == tidcnt)
				tid = 0;
			if (!dd->pageshadow[ctxttid + tid])
				break;
		}
		if (ntids < 0) {
			 
			i--;     
			ret = -ENOMEM;
			break;
		}
		tidlist[i] = tid + tidoff;
		 
		dd->pageshadow[ctxttid + tid] = pagep[i];
		dd->physshadow[ctxttid + tid] =
			qib_map_page(dd->pcidev, pagep[i], 0, PAGE_SIZE,
				     PCI_DMA_FROMDEVICE);
		 
		__set_bit(tid, tidmap);
		physaddr = dd->physshadow[ctxttid + tid];
		 
		dd->f_put_tid(dd, &tidbase[tid],
				  RCVHQ_RCV_TYPE_EXPECTED, physaddr);
		 
		tid++;
	}

	if (ret) {
		u32 limit;
cleanup:
		 
		 
		limit = sizeof(tidmap) * BITS_PER_BYTE;
		if (limit > tidcnt)
			 
			limit = tidcnt;
		tid = find_first_bit((const unsigned long *)tidmap, limit);
		for (; tid < limit; tid++) {
			if (!test_bit(tid, tidmap))
				continue;
			if (dd->pageshadow[ctxttid + tid]) {
				dma_addr_t phys;

				phys = dd->physshadow[ctxttid + tid];
				dd->physshadow[ctxttid + tid] = dd->tidinvalid;
				 
				dd->f_put_tid(dd, &tidbase[tid],
					      RCVHQ_RCV_TYPE_EXPECTED,
					      dd->tidinvalid);
				pci_unmap_page(dd->pcidev, phys, PAGE_SIZE,
					       PCI_DMA_FROMDEVICE);
				dd->pageshadow[ctxttid + tid] = NULL;
			}
		}
		qib_release_user_pages(pagep, cnt);
	} else {
		 
		if (copy_to_user((void __user *)
				 (unsigned long) ti->tidlist,
				 tidlist, cnt * sizeof(*tidlist))) {
			ret = -EFAULT;
			goto cleanup;
		}
		if (copy_to_user((void __user *) (unsigned long) ti->tidmap,
				 tidmap, sizeof(tidmap))) {
			ret = -EFAULT;
			goto cleanup;
		}
		if (tid == tidcnt)
			tid = 0;
		if (!rcd->subctxt_cnt)
			rcd->tidcursor = tid;
		else
			tidcursor_fp(fp) = tid;
	}

done:
	return ret;
}
