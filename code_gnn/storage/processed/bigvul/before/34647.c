static irqreturn_t kvm_assigned_dev_thread(int irq, void *dev_id)
{
	struct kvm_assigned_dev_kernel *assigned_dev = dev_id;
	u32 vector;
	int index;

	if (assigned_dev->irq_requested_type & KVM_DEV_IRQ_HOST_INTX) {
		spin_lock(&assigned_dev->intx_lock);
		disable_irq_nosync(irq);
		assigned_dev->host_irq_disabled = true;
		spin_unlock(&assigned_dev->intx_lock);
	}

	if (assigned_dev->irq_requested_type & KVM_DEV_IRQ_HOST_MSIX) {
		index = find_index_from_host_irq(assigned_dev, irq);
		if (index >= 0) {
			vector = assigned_dev->
					guest_msix_entries[index].vector;
			kvm_set_irq(assigned_dev->kvm,
				    assigned_dev->irq_source_id, vector, 1);
		}
	} else
		kvm_set_irq(assigned_dev->kvm, assigned_dev->irq_source_id,
			    assigned_dev->guest_irq, 1);

	return IRQ_HANDLED;
}
