static int ioctl_send_fib(struct aac_dev * dev, void __user *arg)
{
	struct hw_fib * kfib;
	struct fib *fibptr;
	struct hw_fib * hw_fib = (struct hw_fib *)0;
	dma_addr_t hw_fib_pa = (dma_addr_t)0LL;
	unsigned size;
	int retval;

	if (dev->in_reset) {
		return -EBUSY;
	}
	fibptr = aac_fib_alloc(dev);
	if(fibptr == NULL) {
		return -ENOMEM;
	}

	kfib = fibptr->hw_fib_va;
	 
	if (copy_from_user((void *)kfib, arg, sizeof(struct aac_fibhdr))) {
		aac_fib_free(fibptr);
		return -EFAULT;
	}
	 
	size = le16_to_cpu(kfib->header.Size) + sizeof(struct aac_fibhdr);
	if (size < le16_to_cpu(kfib->header.SenderSize))
		size = le16_to_cpu(kfib->header.SenderSize);
	if (size > dev->max_fib_size) {
		dma_addr_t daddr;

		if (size > 2048) {
			retval = -EINVAL;
			goto cleanup;
		}

		kfib = pci_alloc_consistent(dev->pdev, size, &daddr);
		if (!kfib) {
			retval = -ENOMEM;
			goto cleanup;
		}

		 
		hw_fib = fibptr->hw_fib_va;
		hw_fib_pa = fibptr->hw_fib_pa;
		fibptr->hw_fib_va = kfib;
		fibptr->hw_fib_pa = daddr;
		memset(((char *)kfib) + dev->max_fib_size, 0, size - dev->max_fib_size);
		memcpy(kfib, hw_fib, dev->max_fib_size);
	}

	if (copy_from_user(kfib, arg, size)) {
		retval = -EFAULT;
		goto cleanup;
	}

	if (kfib->header.Command == cpu_to_le16(TakeABreakPt)) {
		aac_adapter_interrupt(dev);
		 
		kfib->header.XferState = 0;
	} else {
		retval = aac_fib_send(le16_to_cpu(kfib->header.Command), fibptr,
				le16_to_cpu(kfib->header.Size) , FsaNormal,
				1, 1, NULL, NULL);
		if (retval) {
			goto cleanup;
		}
		if (aac_fib_complete(fibptr) != 0) {
			retval = -EINVAL;
			goto cleanup;
		}
	}
	 

	retval = 0;
	if (copy_to_user(arg, (void *)kfib, size))
		retval = -EFAULT;
cleanup:
	if (hw_fib) {
		pci_free_consistent(dev->pdev, size, kfib, fibptr->hw_fib_pa);
		fibptr->hw_fib_pa = hw_fib_pa;
		fibptr->hw_fib_va = hw_fib;
	}
	if (retval != -ERESTARTSYS)
		aac_fib_free(fibptr);
	return retval;
}
