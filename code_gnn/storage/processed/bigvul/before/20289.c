int kvm_deassign_device(struct kvm *kvm,
			struct kvm_assigned_dev_kernel *assigned_dev)
{
	struct iommu_domain *domain = kvm->arch.iommu_domain;
	struct pci_dev *pdev = NULL;

	 
	if (!domain)
		return 0;

	pdev = assigned_dev->dev;
	if (pdev == NULL)
		return -ENODEV;

	iommu_detach_device(domain, &pdev->dev);

	pdev->dev_flags &= ~PCI_DEV_FLAGS_ASSIGNED;

	printk(KERN_DEBUG "deassign device %x:%x:%x.%x\n",
		assigned_dev->host_segnr,
		assigned_dev->host_busnr,
		PCI_SLOT(assigned_dev->host_devfn),
		PCI_FUNC(assigned_dev->host_devfn));

	return 0;
}