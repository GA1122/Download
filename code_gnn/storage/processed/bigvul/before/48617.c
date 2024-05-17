void vfio_pci_intx_mask(struct vfio_pci_device *vdev)
{
	struct pci_dev *pdev = vdev->pdev;
	unsigned long flags;

	spin_lock_irqsave(&vdev->irqlock, flags);

	 
	if (unlikely(!is_intx(vdev))) {
		if (vdev->pci_2_3)
			pci_intx(pdev, 0);
	} else if (!vdev->ctx[0].masked) {
		 
		if (vdev->pci_2_3)
			pci_intx(pdev, 0);
		else
			disable_irq_nosync(pdev->irq);

		vdev->ctx[0].masked = true;
	}

	spin_unlock_irqrestore(&vdev->irqlock, flags);
}
