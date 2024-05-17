int kvm_assign_device(struct kvm *kvm,
		      struct kvm_assigned_dev_kernel *assigned_dev)
{
	struct pci_dev *pdev = NULL;
	struct iommu_domain *domain = kvm->arch.iommu_domain;
	int r, last_flags;

	 
	if (!domain)
		return 0;

	pdev = assigned_dev->dev;
	if (pdev == NULL)
		return -ENODEV;

	r = iommu_attach_device(domain, &pdev->dev);
	if (r) {
		printk(KERN_ERR "assign device %x:%x:%x.%x failed",
			pci_domain_nr(pdev->bus),
			pdev->bus->number,
			PCI_SLOT(pdev->devfn),
			PCI_FUNC(pdev->devfn));
		return r;
	}

	last_flags = kvm->arch.iommu_flags;
	if (iommu_domain_has_cap(kvm->arch.iommu_domain,
				 IOMMU_CAP_CACHE_COHERENCY))
		kvm->arch.iommu_flags |= KVM_IOMMU_CACHE_COHERENCY;

	 
	if ((last_flags ^ kvm->arch.iommu_flags) ==
			KVM_IOMMU_CACHE_COHERENCY) {
		kvm_iommu_unmap_memslots(kvm);
		r = kvm_iommu_map_memslots(kvm);
		if (r)
			goto out_unmap;
	}

	pdev->dev_flags |= PCI_DEV_FLAGS_ASSIGNED;

	printk(KERN_DEBUG "assign device %x:%x:%x.%x\n",
		assigned_dev->host_segnr,
		assigned_dev->host_busnr,
		PCI_SLOT(assigned_dev->host_devfn),
		PCI_FUNC(assigned_dev->host_devfn));

	return 0;
out_unmap:
	kvm_iommu_unmap_memslots(kvm);
	return r;
}