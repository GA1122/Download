int kvm_dev_ioctl_check_extension(long ext)
{

	int r;

	switch (ext) {
	case KVM_CAP_IRQCHIP:
	case KVM_CAP_MP_STATE:
	case KVM_CAP_IRQ_INJECT_STATUS:
		r = 1;
		break;
	case KVM_CAP_COALESCED_MMIO:
		r = KVM_COALESCED_MMIO_PAGE_OFFSET;
		break;
	case KVM_CAP_IOMMU:
		r = iommu_present(&pci_bus_type);
		break;
	default:
		r = 0;
	}
	return r;

}
