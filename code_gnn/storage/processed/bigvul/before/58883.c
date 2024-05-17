irqfd_shutdown(struct work_struct *work)
{
	struct kvm_kernel_irqfd *irqfd =
		container_of(work, struct kvm_kernel_irqfd, shutdown);
	u64 cnt;

	 
	eventfd_ctx_remove_wait_queue(irqfd->eventfd, &irqfd->wait, &cnt);

	 
	flush_work(&irqfd->inject);

	if (irqfd->resampler) {
		irqfd_resampler_shutdown(irqfd);
		eventfd_ctx_put(irqfd->resamplefd);
	}

	 
#ifdef CONFIG_HAVE_KVM_IRQ_BYPASS
	irq_bypass_unregister_consumer(&irqfd->consumer);
#endif
	eventfd_ctx_put(irqfd->eventfd);
	kfree(irqfd);
}
