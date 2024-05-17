static int assigned_device_enable_host_intx(struct kvm *kvm,
					    struct kvm_assigned_dev_kernel *dev)
{
	dev->host_irq = dev->dev->irq;
	 
	if (request_threaded_irq(dev->host_irq, NULL, kvm_assigned_dev_thread,
				 IRQF_ONESHOT, dev->irq_name, (void *)dev))
		return -EIO;
	return 0;
}
