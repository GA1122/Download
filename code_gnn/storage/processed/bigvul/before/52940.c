static int qib_mmap_mem(struct vm_area_struct *vma, struct qib_ctxtdata *rcd,
			unsigned len, void *kvaddr, u32 write_ok, char *what)
{
	struct qib_devdata *dd = rcd->dd;
	unsigned long pfn;
	int ret;

	if ((vma->vm_end - vma->vm_start) > len) {
		qib_devinfo(dd->pcidev,
			 "FAIL on %s: len %lx > %x\n", what,
			 vma->vm_end - vma->vm_start, len);
		ret = -EFAULT;
		goto bail;
	}

	 
	if (!write_ok) {
		if (vma->vm_flags & VM_WRITE) {
			qib_devinfo(dd->pcidev,
				 "%s must be mapped readonly\n", what);
			ret = -EPERM;
			goto bail;
		}

		 
		vma->vm_flags &= ~VM_MAYWRITE;
	}

	pfn = virt_to_phys(kvaddr) >> PAGE_SHIFT;
	ret = remap_pfn_range(vma, vma->vm_start, pfn,
			      len, vma->vm_page_prot);
	if (ret)
		qib_devinfo(dd->pcidev,
			"%s ctxt%u mmap of %lx, %x bytes failed: %d\n",
			what, rcd->ctxt, pfn, len, ret);
bail:
	return ret;
}