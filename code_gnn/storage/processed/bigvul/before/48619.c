static int vfio_pci_intx_unmask_handler(void *opaque, void *unused)
{
	struct vfio_pci_device *vdev = opaque;
	struct pci_dev *pdev = vdev->pdev;
	unsigned long flags;
	int ret = 0;

	spin_lock_irqsave(&vdev->irqlock, flags);

	 
	if (unlikely(!is_intx(vdev))) {
		if (vdev->pci_2_3)
			pci_intx(pdev, 1);
	} else if (vdev->ctx[0].masked && !vdev->virq_disabled) {
		 
		if (vdev->pci_2_3) {
			if (!pci_check_and_unmask_intx(pdev))
				ret = 1;
		} else
			enable_irq(pdev->irq);

		vdev->ctx[0].masked = (ret > 0);
	}

	spin_unlock_irqrestore(&vdev->irqlock, flags);

	return ret;
}
