static int vfio_pci_probe(struct pci_dev *pdev, const struct pci_device_id *id)
{
	struct vfio_pci_device *vdev;
	struct iommu_group *group;
	int ret;

	if (pdev->hdr_type != PCI_HEADER_TYPE_NORMAL)
		return -EINVAL;

	group = vfio_iommu_group_get(&pdev->dev);
	if (!group)
		return -EINVAL;

	vdev = kzalloc(sizeof(*vdev), GFP_KERNEL);
	if (!vdev) {
		vfio_iommu_group_put(group, &pdev->dev);
		return -ENOMEM;
	}

	vdev->pdev = pdev;
	vdev->irq_type = VFIO_PCI_NUM_IRQS;
	mutex_init(&vdev->igate);
	spin_lock_init(&vdev->irqlock);

	ret = vfio_add_group_dev(&pdev->dev, &vfio_pci_ops, vdev);
	if (ret) {
		vfio_iommu_group_put(group, &pdev->dev);
		kfree(vdev);
		return ret;
	}

	if (vfio_pci_is_vga(pdev)) {
		vga_client_register(pdev, vdev, NULL, vfio_pci_set_vga_decode);
		vga_set_legacy_decoding(pdev,
					vfio_pci_set_vga_decode(vdev, false));
	}

	if (!disable_idle_d3) {
		 
		pci_set_power_state(pdev, PCI_D0);
		pci_set_power_state(pdev, PCI_D3hot);
	}

	return ret;
}