static int hfi1_file_mmap(struct file *fp, struct vm_area_struct *vma)
{
	struct hfi1_filedata *fd = fp->private_data;
	struct hfi1_ctxtdata *uctxt = fd->uctxt;
	struct hfi1_devdata *dd;
	unsigned long flags, pfn;
	u64 token = vma->vm_pgoff << PAGE_SHIFT,
		memaddr = 0;
	u8 subctxt, mapio = 0, vmf = 0, type;
	ssize_t memlen = 0;
	int ret = 0;
	u16 ctxt;

	if (!is_valid_mmap(token) || !uctxt ||
	    !(vma->vm_flags & VM_SHARED)) {
		ret = -EINVAL;
		goto done;
	}
	dd = uctxt->dd;
	ctxt = HFI1_MMAP_TOKEN_GET(CTXT, token);
	subctxt = HFI1_MMAP_TOKEN_GET(SUBCTXT, token);
	type = HFI1_MMAP_TOKEN_GET(TYPE, token);
	if (ctxt != uctxt->ctxt || subctxt != fd->subctxt) {
		ret = -EINVAL;
		goto done;
	}

	flags = vma->vm_flags;

	switch (type) {
	case PIO_BUFS:
	case PIO_BUFS_SOP:
		memaddr = ((dd->physaddr + TXE_PIO_SEND) +
				 
			   (uctxt->sc->hw_context * BIT(16))) +
				 
			(type == PIO_BUFS_SOP ?
				(TXE_PIO_SIZE / 2) : 0);  
		 
		memlen = PAGE_ALIGN(uctxt->sc->credits * PIO_BLOCK_SIZE);
		flags &= ~VM_MAYREAD;
		flags |= VM_DONTCOPY | VM_DONTEXPAND;
		vma->vm_page_prot = pgprot_writecombine(vma->vm_page_prot);
		mapio = 1;
		break;
	case PIO_CRED:
		if (flags & VM_WRITE) {
			ret = -EPERM;
			goto done;
		}
		 
		memaddr = dd->cr_base[uctxt->numa_id].pa +
			(((u64)uctxt->sc->hw_free -
			  (u64)dd->cr_base[uctxt->numa_id].va) & PAGE_MASK);
		memlen = PAGE_SIZE;
		flags &= ~VM_MAYWRITE;
		flags |= VM_DONTCOPY | VM_DONTEXPAND;
		 
		 
		mapio = 1;
		break;
	case RCV_HDRQ:
		memaddr = uctxt->rcvhdrq_phys;
		memlen = uctxt->rcvhdrq_size;
		break;
	case RCV_EGRBUF: {
		unsigned long addr;
		int i;
		 
		memlen = uctxt->egrbufs.size;
		if ((vma->vm_end - vma->vm_start) != memlen) {
			dd_dev_err(dd, "Eager buffer map size invalid (%lu != %lu)\n",
				   (vma->vm_end - vma->vm_start), memlen);
			ret = -EINVAL;
			goto done;
		}
		if (vma->vm_flags & VM_WRITE) {
			ret = -EPERM;
			goto done;
		}
		vma->vm_flags &= ~VM_MAYWRITE;
		addr = vma->vm_start;
		for (i = 0 ; i < uctxt->egrbufs.numbufs; i++) {
			ret = remap_pfn_range(
				vma, addr,
				uctxt->egrbufs.buffers[i].phys >> PAGE_SHIFT,
				uctxt->egrbufs.buffers[i].len,
				vma->vm_page_prot);
			if (ret < 0)
				goto done;
			addr += uctxt->egrbufs.buffers[i].len;
		}
		ret = 0;
		goto done;
	}
	case UREGS:
		 
		memaddr = (unsigned long)
			(dd->physaddr + RXE_PER_CONTEXT_USER)
			+ (uctxt->ctxt * RXE_PER_CONTEXT_SIZE);
		 
		memlen = PAGE_SIZE;
		flags |= VM_DONTCOPY | VM_DONTEXPAND;
		vma->vm_page_prot = pgprot_noncached(vma->vm_page_prot);
		mapio = 1;
		break;
	case EVENTS:
		 
		memaddr = (unsigned long)(dd->events +
					  ((uctxt->ctxt - dd->first_user_ctxt) *
					   HFI1_MAX_SHARED_CTXTS)) & PAGE_MASK;
		memlen = PAGE_SIZE;
		 
		flags |= VM_IO | VM_DONTEXPAND;
		vmf = 1;
		break;
	case STATUS:
		memaddr = kvirt_to_phys((void *)dd->status);
		memlen = PAGE_SIZE;
		flags |= VM_IO | VM_DONTEXPAND;
		break;
	case RTAIL:
		if (!HFI1_CAP_IS_USET(DMA_RTAIL)) {
			 
			ret = -EINVAL;
			goto done;
		}
		if (flags & VM_WRITE) {
			ret = -EPERM;
			goto done;
		}
		memaddr = uctxt->rcvhdrqtailaddr_phys;
		memlen = PAGE_SIZE;
		flags &= ~VM_MAYWRITE;
		break;
	case SUBCTXT_UREGS:
		memaddr = (u64)uctxt->subctxt_uregbase;
		memlen = PAGE_SIZE;
		flags |= VM_IO | VM_DONTEXPAND;
		vmf = 1;
		break;
	case SUBCTXT_RCV_HDRQ:
		memaddr = (u64)uctxt->subctxt_rcvhdr_base;
		memlen = uctxt->rcvhdrq_size * uctxt->subctxt_cnt;
		flags |= VM_IO | VM_DONTEXPAND;
		vmf = 1;
		break;
	case SUBCTXT_EGRBUF:
		memaddr = (u64)uctxt->subctxt_rcvegrbuf;
		memlen = uctxt->egrbufs.size * uctxt->subctxt_cnt;
		flags |= VM_IO | VM_DONTEXPAND;
		flags &= ~VM_MAYWRITE;
		vmf = 1;
		break;
	case SDMA_COMP: {
		struct hfi1_user_sdma_comp_q *cq = fd->cq;

		if (!cq) {
			ret = -EFAULT;
			goto done;
		}
		memaddr = (u64)cq->comps;
		memlen = PAGE_ALIGN(sizeof(*cq->comps) * cq->nentries);
		flags |= VM_IO | VM_DONTEXPAND;
		vmf = 1;
		break;
	}
	default:
		ret = -EINVAL;
		break;
	}

	if ((vma->vm_end - vma->vm_start) != memlen) {
		hfi1_cdbg(PROC, "%u:%u Memory size mismatch %lu:%lu",
			  uctxt->ctxt, fd->subctxt,
			  (vma->vm_end - vma->vm_start), memlen);
		ret = -EINVAL;
		goto done;
	}

	vma->vm_flags = flags;
	hfi1_cdbg(PROC,
		  "%u:%u type:%u io/vf:%d/%d, addr:0x%llx, len:%lu(%lu), flags:0x%lx\n",
		    ctxt, subctxt, type, mapio, vmf, memaddr, memlen,
		    vma->vm_end - vma->vm_start, vma->vm_flags);
	pfn = (unsigned long)(memaddr >> PAGE_SHIFT);
	if (vmf) {
		vma->vm_pgoff = pfn;
		vma->vm_ops = &vm_ops;
		ret = 0;
	} else if (mapio) {
		ret = io_remap_pfn_range(vma, vma->vm_start, pfn, memlen,
					 vma->vm_page_prot);
	} else {
		ret = remap_pfn_range(vma, vma->vm_start, pfn, memlen,
				      vma->vm_page_prot);
	}
done:
	return ret;
}